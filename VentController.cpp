// VentController.cpp
#include "VentController.hpp"
#include "Vent.hpp"
#include "Arduino.h"

VentController::VentController(const int         *pins,
                               const unsigned int count,
                               const int          master_pin) :
  _vent_count(count) {
  if (count % 2 != 0) {
    return; // not of reversible type
  }

  // Create empty array for roof windows (is fine to dynamically allocate
  // because is used for whole program lifetime)
  _vents = new Vent *[_vent_count];

  _state.reserve(_vent_count);

  for (int i = 0; i < _vent_count; i++) {
    _vents[i] = new Vent(pins[2 * i],
                         pins[2 * i + 1],
                         master_pin);

    _state += 'u';
  }
}

void VentController::set_pattern(String pattern) {
  int pattern_length = pattern.length();

  // validity checks
  if (pattern_length != _vent_count) {
    return; // invalid pattern
  }

  for (int i = 0; i < pattern_length; i++) {
    char current_character = pattern.charAt(i);

    if ((current_character != 'o') && (current_character != '-')) {
      return; // invalid pattern
    }
  }

  // apply pattern
  for (int i = 0; i < pattern_length; i++) {
    char current_character = pattern.charAt(i);

    switch (current_character) {
    case '-':
      _vents[i]->actuate(0);
      break;

    case 'o':
      _vents[i]->actuate(1);
      break;
    }
  }
}

String VentController::get_state() {
  return _state;
}