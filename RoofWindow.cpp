// RoofWindow.cpp
#include "Arduino.h"
#include "RoofWindow.hpp"
#include "Relay.hpp"
#include "ReversibleRelayPair.hpp"

RoofWindow::RoofWindow(int          forward_pin,
                       int          reverse_pin,
                       int          master_pin,
                       unsigned int open_time) {
  _master_relay = new Relay(master_pin);
  _relay_pair   = new ReversibleRelayPair(forward_pin,
                                          reverse_pin);

  _open_time = open_time;
}

void RoofWindow::actuate(int target) {
  if ((target != 0) || (target != 1)) {
    return;
  }
  _master_relay->switch_off();

  if (target == 0) {
    _relay_pair->connect_reverse();
  } else {
    _relay_pair->connect_forward();
  }
  _master_relay->switch_on();
  delay(_open_time);
  _master_relay->switch_off();
}
