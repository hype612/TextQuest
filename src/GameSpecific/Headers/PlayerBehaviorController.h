#ifndef PLAYERBEHAVIOURCONTROLLER_H
#define PLAYERBEHAVIOURCONTROLLER_H

#include "IBehaviorController.h"
#include "IHealthObserver.h"
#include "IInputHandler.h"
#include "IShootObserver.h"
#include "MoveDirection.h"

class PlayerBehaviorController : public IBehaviorController {
public:
  PlayerBehaviorController(IInputHandler &input);
  vec2f Tick(Entity &self, float delta) override;
  void onHit(Entity &self, Entity &other) override;

  // Non-owning; the observer must outlive this controller (or be reset to
  // nullptr first).
  void setHealthObserver(IHealthObserver *observer) {
    _healthObserver = observer;
  }
  void setShootObserver(IShootObserver *observer) { _shootObserver = observer; }

private:
  float _sinceLastShot = 0.f;
  const float _shootcd = 0.1f;
  vec2f moveIntent(MoveDirection dir, Entity &self, float delta);
  IInputHandler &_inputHandler;
  IHealthObserver *_healthObserver = nullptr;
  IShootObserver *_shootObserver = nullptr;
};

#endif // PLAYERBEHAVIOURCONTROLLER_H
