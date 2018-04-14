// ReversibleRelayPair.cpp
#include "ReversibleRelayPair.hpp"
#include "Arduino.h"
ReversibleRelayPair::ReversibleRelayPair(int forward_relay_pin,
                                         int reverse_relay_pin) :
  _forward_relay_pin(forward_relay_pin), _reverse_relay_pin(reverse_relay_pin) {
  // Initialisation
  digitalWrite(_forward_relay_pin, HIGH);
  pinMode(_forward_relay_pin, OUTPUT);
  digitalWrite(_reverse_relay_pin, HIGH);
  pinMode(_reverse_relay_pin, OUTPUT);
  // delay(100);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,        LOW);
  digitalWrite(_forward_relay_pin, HIGH);
  digitalWrite(_reverse_relay_pin, HIGH);
}

void ReversibleRelayPair::connect_forward() {
  disconnect_both();
  digitalWrite(_forward_relay_pin, LOW);
  digitalWrite(_reverse_relay_pin, HIGH);
  state = 1;
  delay(switch_time);
}

void ReversibleRelayPair::connect_reverse() {
  disconnect_both();
  digitalWrite(_reverse_relay_pin, LOW);
  digitalWrite(_forward_relay_pin, HIGH);
  state = -1;
  delay(switch_time);
}

void ReversibleRelayPair::disconnect_both() {
  digitalWrite(_forward_relay_pin, HIGH);
  digitalWrite(_reverse_relay_pin, HIGH);
  state = 0;
  delay(switch_time);
}
