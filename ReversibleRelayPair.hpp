// ReversibleRelayPair.h
#ifndef REVERSIBLE_RELAY_PAIR_H
#define REVERSIBLE_RELAY_PAIR_H

class ReversibleRelayPair {
private:

  unsigned int switch_time = 500;

protected:

  int _forward_relay_pin, _reverse_relay_pin;

public:

  ReversibleRelayPair(int forward_relay_pin, int reverse_relay_pin);
  int state = 0;
  void connect_forward();
  void connect_reverse();
  void disconnect_both();
};

#endif /* ifndef REVERSIBLE_RELAY_PAIR_H */
