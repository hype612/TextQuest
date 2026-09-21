#ifndef ISTVANBEHAVIORCONTROLLER_H
#define ISTVANBEHAVIORCONTROLLER_H

#include "../Headers/MoveDirection.h"
#include "IBehaviorController.h"
#include "ICollidable.h"

class IstvanBehaviorController : public IBehaviorController {
public:
  IstvanBehaviorController();
  vec2f Tick(Entity &self, float delta) override;
  void onCollision(Entity &self, ICollidable &other) override;
  virtual void onVisible(Entity &self, Entity &other) override;
  virtual void onHit(Entity &self, Entity &other) override;

private:
  enum class State { IDLE, CHASE, ATTACK };
  State _state;
  /*
   * For Istvan, the valid state-changes are:
   * IDLE -> CHASE
   * IDLE -> ATTACK
   * CHASE -> ATTACK
   * ATTACK -> CHASE
   * NOTE: Istvan cannot go back to idle once he
   * sees the player
   */
  void updateState(Entity &self);

  vec2f moveIntent(MoveDirection dir, Entity &self, float delta);
  Entity *_targetE;
  vec2f _target;
  float _targetAngle;
  bool _hasTarget;
  float _lastShot = 0.f;
  const float _shootCooldown = 0.3f;
};

#endif // ISTVANBEHAVIORCONTROLLER_H
