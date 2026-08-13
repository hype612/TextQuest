#ifndef IBEHAVIOURCONTROLLER_H
#define IBEHAVIOURCONTROLLER_H

#include "ICollidable.h"
#include "Vec2f.h"

class Entity;

class IBehaviorController {
public:
  virtual ~IBehaviorController() = default;
  virtual vec2f Tick(Entity &self, float delta) = 0;
  virtual void onCollision(Entity &, ICollidable &) {}
};

#endif // IBEHAVIOURCONTROLLER_H
