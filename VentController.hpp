// VentController.h
#ifndef VENT_CONTROLLER_H
#define VENT_CONTROLLER_H

#include "Arduino.h"
#include "Vent.hpp"

class VentController {
public:

  VentController(const String id,
                 const int *pins,
                 const unsigned int count,
                 const int master_pin);
  void   set_pattern(String pattern);
  String get_state();

private:

  Vent **_vents;
  int    _vent_count = 0;
  String _state;
  String _id;
};

#endif /* ifndef VENT_CONTROLLER_H */
