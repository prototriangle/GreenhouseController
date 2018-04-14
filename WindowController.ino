#include <OneWire.h>
#include <DallasTemperature.h>

#include "VentController.hpp"

#define ONE_WIRE_BUS 53
#define TEMPERATURE_PRECISION 9 // Lower resolution


OneWire           oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(& oneWire);
int number_of_devices; // Number of temperature devices

int temp_to_open  = 30;
int temp_to_close = 26;

float t_cur;
float t_prev;

int lock = 0;

bool   testing_mode = false;
String last_input;

const int roof_window_master_relay_pin = 12;
const int roof_window_relay_pins[]     = {
  11, 10, // Roof Window 1
  9,  8,  // Roof Window 2
  7,  6,  // Roof Window 3
  5,  4,  // Roof Window 4
  3,  2,  // Roof Window 5
  14, 15, // Roof Window 6
  16, 17, // Roof Window 7
  18, 19  // Roof Window 8
};
const unsigned int roof_window_count = 8;

const int louvre_vent_master_relay_pin = 41;
const int louvre_vent_relay_pins[]     = {
  22, 23, // Louvre Vent 1
  24, 25, // Louvre Vent 2
  26, 27, // Louvre Vent 3
  28, 29, // Louvre Vent 4
  30, 31, // Louvre Vent 5
  32, 33  // Louvre Vent 6
};
const unsigned int louvre_vent_count = 6;

const int fan_master_relay_pin = 40;
const int fan_relay_pins[]     = {
  34, // Fan 1
  35, // Fan 2
  36, // Fan 3
  37  // Fan 4
};
const unsigned int fan_count = 4;

const unsigned int pattern_count = 8;
String patterns[]                = {
  "--------|-/--/-/--", // closed
  "--o--o--|-/--/-/--", // level 1
  "--o--o--|-/-o/o/--", // level 2
  "-oo--oo-|-/-o/o/--", // level 3
  "-oo--oo-|o/-o/o/-o", // level 4
  "ooo--ooo|o/-o/o/-o", // level 5
  "oooooooo|o/-o/o/-o", // level 6
  "oooooooo|o/oo/o/oo"  // level 7
};


int current_pattern_index = -1;

float temp_thresh_must_close  = 25.0;
float temp_thresh_start_close = 27.0;
float temp_thresh_open        = 30.0;

int open_time   = 10000;
int switch_time = 500;

const int all_open_pin   = A5;
const int all_closed_pin = A6;

VentController roof_window_controller = VentController(
  roof_window_relay_pins,
  roof_window_count,
  roof_window_master_relay_pin);

VentController louvre_vent_controller = VentController(
  louvre_vent_relay_pins,
  louvre_vent_count,
  louvre_vent_master_relay_pin);

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    // wait for serial port to connect. Needed for native USB port only
  }

  sensors.begin();
  number_of_devices = sensors.getDeviceCount();

  pinMode(all_open_pin,   INPUT_PULLUP);
  pinMode(all_closed_pin, INPUT_PULLUP);

  sensors.requestTemperatures();
  float sum = 0.0;

  for (int i = 0; i < number_of_devices; i++) {
    float t = sensors.getTempCByIndex(i);
    sum += t;
  }
  t_prev = sum / (float)number_of_devices;
  Serial.println("Begin");
  Serial.print("temp. sensor count: ");
  Serial.println(number_of_devices);
}

unsigned long next_loop_millis = 0UL;
unsigned long period           = 180000UL;
unsigned long testing_period   = 20000UL;


void loop() {
  unsigned long startMillis = millis();

    readSerialString();

  if (startMillis > next_loop_millis) {
    Serial.println("Running main loop...");
    Serial.print("Previous temp.: ");
    Serial.println(t_prev);
    next_loop_millis = testing_mode ? startMillis +
                       testing_period : startMillis + period;
    readSerialString();

    t_cur = getNewAverageTemperature();

    if (t_cur <= temp_thresh_must_close) {
      String pattern = patterns[0];
      current_pattern_index = 1;
      roof_window_controller.set_pattern(get_roof_window_pattern(pattern));
      louvre_vent_controller.set_pattern(get_louvre_vent_pattern(pattern));
    }
    else if (t_cur >= temp_thresh_open) {
      if (current_pattern_index <= 0) {
        String pattern = patterns[1];
        current_pattern_index = 0;
        roof_window_controller.set_pattern(get_roof_window_pattern(pattern));
        louvre_vent_controller.set_pattern(get_louvre_vent_pattern(pattern));
      }
      else if ((current_pattern_index > 0) &&
               (current_pattern_index < pattern_count - 1) &&
               (t_cur - t_prev >= 0)) {
        current_pattern_index += 1;
        String pattern = patterns[current_pattern_index];
        roof_window_controller.set_pattern(get_roof_window_pattern(pattern));
        louvre_vent_controller.set_pattern(get_louvre_vent_pattern(pattern));
      }
    }
    else if (t_cur <= temp_thresh_start_close) {
      if ((current_pattern_index > 0) && (t_cur - t_prev < 0)) {
        current_pattern_index -= 1;
        String pattern = patterns[current_pattern_index];
        roof_window_controller.set_pattern(get_roof_window_pattern(pattern));
        louvre_vent_controller.set_pattern(get_louvre_vent_pattern(pattern));
      }
    }

    t_prev = t_cur;
    Serial.println("Main loop done");
  }
  delay(40);
}

float getNewAverageTemperature() {
  if (!testing_mode) {
    sensors.requestTemperatures(); // Send the command to get temperatures
    float sum = 0.0;

    Serial.print("Temperatures: ");

    for (int i = 0; i < number_of_devices; i++) {
      float t = sensors.getTempCByIndex(i);
      Serial.print(t);
      Serial.print(" | ");
      sum += t;
    }
    float avg = sum / (float)number_of_devices;
    Serial.print("\nAverage: ");
    Serial.println(avg);
    return avg;
  }
  float t = last_input.toFloat();
  Serial.print("Temperature: ");
  Serial.println(t);
  return t;
}

void readSerialString() {
  String str = "";

  while (Serial.available() > 0) {
    str += (char)Serial.read();
  }

  if (str.length() > 0) {
    Serial.println("Input: " + str);

    if (str.equals("testing")) {
      testing_mode = true;
      Serial.println("Entering testing mode...");
      next_loop_millis = millis();
    } else if (str.equals("end testing")) {
      testing_mode = false;
      Serial.println("Leaving testing mode...");
      next_loop_millis = millis();
    } else if (str.equals("index")) {
      Serial.print("Pattern index: ");
      Serial.println(current_pattern_index);
      Serial.println(patterns[current_pattern_index]);
    }
    last_input = str;
  }
}

String get_roof_window_pattern(String pattern) {
  return pattern.substring(0, roof_window_count);
}

String get_louvre_vent_pattern(String pattern) {
  String str = pattern.substring(roof_window_count + 1);
  str.remove(1, 1);
  str.remove(3, 1);
  str.remove(4, 1);
  return str;
}
