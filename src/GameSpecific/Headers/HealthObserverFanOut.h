#ifndef HEALTHOBSERVERFANOUT_H
#define HEALTHOBSERVERFANOUT_H

#include "IHealthObserver.h"
#include <vector>

class HealthObserverFanOut : public IHealthObserver {
public:
  void add(IHealthObserver *o) { _observers.push_back(o); }
  void onHealthChanged(int current) override {
    for (IHealthObserver *o : _observers)
      o->onHealthChanged(current);
  }

private:
  std::vector<IHealthObserver *> _observers;
};

#endif // HEALTHOBSERVERFANOUT_H
