// RoofWindow.h
#ifndef ROOF_WINDOW_H
#define ROOF_WINDOW_H

#include "Actuator.hpp"
#include "Relay.hpp"
#include "ReversibleRelayPair.hpp"

class RoofWindow : public Actuator {
private:

  ReversibleRelayPair *_relay_pair;
  Relay *_master_relay;
  int    _open_time;

public:

  RoofWindow(int forward_pin,
             int reverse_pin,
             int master_pin,
             unsigned int open_time = 10000);
  void actuate(int target);
};

#endif /* ifndef ROOF_WINDOW_H */

