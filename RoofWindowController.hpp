// RoofWindowController.h
#ifndef ROOF_WINDOW_CONTROLLER_H
#define ROOF_WINDOW_CONTROLLER_H

#include "Arduino.h"
#include "RoofWindow.hpp"

class RoofWindowController {
public:

  RoofWindowController(const int *pins,
                       const unsigned int count,
                       const int master_pin);
  void set_pattern(String pattern);
  String get_state();

private:

  RoofWindow **_roof_windows;
  int _roof_window_count = 0;
  String _state;
};

#endif /* ifndef ROOF_WINDOW_CONTROLLER_H */
