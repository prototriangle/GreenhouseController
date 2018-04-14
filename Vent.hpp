// Vent.h
#ifndef VENT_H
#define VENT_H

#include "Actuator.hpp"
#include "Relay.hpp"
#include "ReversibleRelayPair.hpp"

class Vent : public Actuator {
private:

  ReversibleRelayPair *_relay_pair;
  Relay *_master_relay;
  int    _open_time;

public:

  Vent(int forward_pin,
       int reverse_pin,
       int master_pin,
       unsigned int open_time = 10000);
  void actuate(int target);
};

#endif /* ifndef VENT_H */
