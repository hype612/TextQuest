#include "../Headers/PlayerBehaviorController.h"
#include "../../Headers/Entity.h"
#include "MoveDirection.h"

#include <cmath>
#include <ncurses.h>
#include <numbers>

PlayerBehaviorController::PlayerBehaviorController(IInputHandler &input)
    : _inputHandler(input) {}

vec2f PlayerBehaviorController::Tick(Entity &self, float delta) {
  self.setShooting(false);

  vec2f moveDelta{0.f, 0.f};
  if (_inputHandler.keyDown(MoveDirection::FORWARD)) {
    moveDelta += moveIntent(MoveDirection::FORWARD, self, delta);
  }
  if (_inputHandler.keyDown(MoveDirection::BACKWARD)) {
    moveDelta += moveIntent(MoveDirection::BACKWARD, self, delta);
  }
  if (_inputHandler.keyDown(MoveDirection::STRAFE_LEFT)) {
    moveDelta += moveIntent(MoveDirection::STRAFE_LEFT, self, delta);
  }
  if (_inputHandler.keyDown(MoveDirection::STRAFE_RIGHT)) {
    moveDelta += moveIntent(MoveDirection::STRAFE_RIGHT, self, delta);
  }
  if (_inputHandler.keyDown(MoveDirection::TURN_LEFT)) {
    moveDelta += moveIntent(MoveDirection::TURN_LEFT, self, delta);
  }
  if (_inputHandler.keyDown(MoveDirection::TURN_RIGHT)) {
    moveDelta += moveIntent(MoveDirection::TURN_RIGHT, self, delta);
  }
  if (_inputHandler.keyDown(MoveDirection::SHOOT)) {
    moveDelta += moveIntent(MoveDirection::SHOOT, self, delta);
  } else {
    self.setShooting(false);
  }
  return self.transform().position + moveDelta;
}

vec2f PlayerBehaviorController::moveIntent(MoveDirection dir, Entity &self,
                                           float delta) {
  switch (dir) {
  case MoveDirection::FORWARD:
    return {sinf(self.transform().angle) * self.moveSpeedOnDir(dir) * delta,
            cosf(self.transform().angle) * self.moveSpeedOnDir(dir) * delta};
    break;
  case MoveDirection::BACKWARD:
    return {
        -1 * sinf(self.transform().angle) * self.moveSpeedOnDir(dir) * delta,
        -1 * cosf(self.transform().angle) * self.moveSpeedOnDir(dir) * delta};
    break;
  case MoveDirection::STRAFE_LEFT:
    return {sinf(self.transform().angle - (std::numbers::pi_v<float> / 2.f)) *
                self.moveSpeedOnDir(dir) * delta,
            cosf(self.transform().angle - (std::numbers::pi_v<float> / 2.f)) *
                self.moveSpeedOnDir(dir) * delta};

    break;
  case MoveDirection::STRAFE_RIGHT:
    return {sinf(self.transform().angle + (std::numbers::pi_v<float> / 2.f)) *
                self.moveSpeedOnDir(dir) * delta,
            cosf(self.transform().angle + (std::numbers::pi_v<float> / 2.f)) *
                self.moveSpeedOnDir(dir) * delta};

    break;
  case MoveDirection::TURN_LEFT:
    self.addToAngle(-1 * self.moveSpeedOnDir(dir) * delta);
    return {0.f, 0.f};
    break;
  case MoveDirection::TURN_RIGHT:
    self.addToAngle(self.moveSpeedOnDir(dir) * delta);
    return {0.f, 0.f};
    break;
  case MoveDirection::SHOOT:
    _sinceLastShot += delta;
    if (_sinceLastShot >= _shootcd) {
      self.setShooting(true);
      _sinceLastShot = 0.f;
    }
    return {0.f, 0.f};
    break;
  }
  return {0.f, 0.f};
}
