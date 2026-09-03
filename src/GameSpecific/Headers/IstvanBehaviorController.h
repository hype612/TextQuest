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
  virtual void onVisible(Entity &self, Entity &other) override;
  virtual void
  onHit(Entity &self,
        Entity &other) override; // intended to query the dmg on hit

private:
  vec2f moveIntent(MoveDirection dir, Entity &self, float delta);
  std::array<MoveDirection, 2> _alternates;
  int _a_idx;
  vec2f _target;
  float _targetAngle;
};

#endif // ISTVANBEHAVIORCONTROLLER_H
