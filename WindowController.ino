#include <OneWire.h>
#include <DallasTemperature.h>

#include "RoofWindowController.hpp"

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

char roof_window_states[] = {
  'u',
  'u',
  'u',
  'u',
  'u',
  'u',
  'u',
  'u'
};


const unsigned int roof_window_pattern_count = 7;
String roof_window_patterns[]                = {
  "cccccccc", // closed
  "occcccco", // level 1
  "ooccccoo", // level 2
  "oooccooo", // level 3
  "oooooooo"  // level 4
};

const unsigned int louvre_vent_pattern_count = 5;
String louvre_vent_patterns[]                = {
  "cccccc", // closed
  "occcco", // level 1
  "ocooco", // level 2
  "oooooo"  // level 3
};

int current_roof_window_pattern_index = -1;

float temp_thresh_must_close  = 25.0;
float temp_thresh_start_close = 27.0;
float temp_thresh_open        = 30.0;

int open_time   = 10000;
int switch_time = 500;

const int all_open_pin   = A5;
const int all_closed_pin = A6;

RoofWindowController roof_window_controller = RoofWindowController(
  roof_window_relay_pins,
  roof_window_count,
  roof_window_master_relay_pin);

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

void loop() {
  readSerialString();
  t_cur = getNewAverageTemperature();

  if (t_cur <= temp_thresh_must_close) {
    roof_window_controller.set_pattern("cccccccc");
  }
  else if (t_cur >= temp_thresh_open) {
    if (current_roof_window_pattern_index <= 0) {
      roof_window_controller.set_pattern(roof_window_patterns[1]);
    }
    else if ((current_roof_window_pattern_index > 0) &&
             (current_roof_window_pattern_index < roof_window_pattern_count -
              1) &&
             (t_cur - t_prev >= 0)) {
      roof_window_controller.set_pattern(
        roof_window_patterns[++
                             current_roof_window_pattern_index
        ]);
    }
  }
  else if (t_cur <= temp_thresh_start_close) {
    if ((current_roof_window_pattern_index > 0) && (t_cur - t_prev < 0)) {
      roof_window_controller.set_pattern(
        roof_window_patterns[--
                             current_roof_window_pattern_index
        ]);
    }
  }

  t_prev = t_cur;

  if (t_cur < temp_thresh_must_close) {} else if (t_cur > temp_thresh_open) {}

  for (int i = 0; i < 18; i++) {
    delay(100);
    float temp_t_cur = getNewAverageTemperature();
  }
}

float getNewAverageTemperature() {
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

void readSerialString() {
  String str = "";

  while (Serial.available() > 0) {
    str += (char)Serial.read();
  }

  if (str.length() > 0) {
    Serial.println(str);
  }
}
