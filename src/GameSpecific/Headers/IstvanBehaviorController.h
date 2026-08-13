#ifndef ISTVANBEHAVIORCONTROLLER_H
#define ISTVANBEHAVIORCONTROLLER_H

#include "../Headers/MoveDirection.h"
#include "IBehaviorController.h"
#include "ICollidable.h"
#include <array>

class IstvanBehaviorController : public IBehaviorController {
public:
  IstvanBehaviorController();
  vec2f Tick(Entity &self, float delta) override;
  void onCollision(Entity &self, ICollidable &other) override;

private:
  vec2f moveIntent(MoveDirection dir, Entity &self, float delta);
  std::array<MoveDirection, 2> _alternates;
  int _a_idx;
};

#endif // ISTVANBEHAVIORCONTROLLER_H
