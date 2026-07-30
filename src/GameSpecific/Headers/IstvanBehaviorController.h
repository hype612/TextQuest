#ifndef ISTVANBEHAVIORCONTROLLER_H
#define ISTVANBEHAVIORCONTROLLER_H

#include "IBehaviorController.h"

class IstvanBehaviorController : public IBehaviorController {
public:
  IstvanBehaviorController() = default;
  vec2f Tick(Entity &self, float delta) override;

private:
};

#endif // ISTVANBEHAVIORCONTROLLER_H
