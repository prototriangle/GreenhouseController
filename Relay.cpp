// Relay.cpp
#include "Relay.hpp"
#include "Arduino.h"
Relay::Relay(int pin) {
  _pin = pin;
  digitalWrite(pin, HIGH);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  state = 1;
}

void Relay::switch_on() {
  digitalWrite(_pin, HIGH);
  delay(switch_time);
}

void Relay::switch_off() {
  digitalWrite(_pin, LOW);
  delay(switch_time);
}

void Relay::switch_state() {
  int val = state == 1 ? HIGH : LOW;

  digitalWrite(_pin, val);
  delay(switch_time);
}
