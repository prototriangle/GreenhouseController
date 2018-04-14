// Relay.h
#ifndef RELAY_H
#define RELAY_H

class Relay {
private:

  unsigned int switch_time = 100;
  int _pin;

public:

  Relay(int pin);
  int state = -1;
  void switch_on();
  void switch_off();
  void switch_state();
};

#endif /* ifndef RELAY_H */
