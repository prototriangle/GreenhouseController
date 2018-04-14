// RoofWindowController.cpp
#include "RoofWindowController.hpp"
#include "Arduino.h"

RoofWindowController::RoofWindowController(const int         *pins,
                                           const unsigned int count,
                                           const int          master_pin) :
  _roof_window_count(count) {
  if (count % 2 != 0) {
    return; // not of reversible type
  }

  // Create empty array for roof windows (is fine to dynamically allocate
  // because is used for whole program lifetime)
  _roof_windows = new RoofWindow *[_roof_window_count];

  _state.reserve(_roof_window_count);

  for (int i = 0; i < _roof_window_count; i ++) {
    _roof_windows[i] = new RoofWindow(pins[2 * i],
                                      pins[2 * i + 1],
                                      master_pin);

    _state += 'u';
  }
}

void RoofWindowController::set_pattern(String pattern) {
  int pattern_length = pattern.length();

  // validity checks
  if (pattern_length != _roof_window_count) {
    return; // invalid pattern
  }

  for (int i = 0; i < pattern_length; i++) {
    char current_character = pattern.charAt(i);

    if ((current_character != 'o') && (current_character != 'c')) {
      return; // invalid pattern
    }
  }

  // apply pattern
  for (int i = 0; i < pattern_length; i++) {
    char current_character = pattern.charAt(i);

    switch (current_character) {
    case 'c':
      _roof_windows[i]->actuate(0);
      break;

    case 'o':
      _roof_windows[i]->actuate(1);
      break;
    }
  }
}
