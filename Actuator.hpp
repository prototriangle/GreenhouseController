// Actuator.hpp
#ifndef ACTUATOR_H
#define ACTUATOR_H

class Actuator {
public:

  virtual void actuate(int target) {}

  virtual void actuate()           {}
};

#endif /* ifndef ACTUATOR_H */
