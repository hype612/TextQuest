#ifndef PLAYERBEHAVIOURCONTROLLER_H
#define PLAYERBEHAVIOURCONTROLLER_H

#include "IBehaviorController.h"
#include "IInputHandler.h"
#include "MoveDirection.h"

class PlayerBehaviorController : public IBehaviorController {
public:
  PlayerBehaviorController(IInputHandler &input);
  vec2f Tick(Entity &self, float delta) override;

private:
  vec2f moveIntent(MoveDirection dir, Entity &self, float delta);
  IInputHandler &_inputHandler;
};

#endif // PLAYERBEHAVIOURCONTROLLER_H
