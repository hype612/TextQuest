#include "../Headers/PlayerBehaviorController.h"
#include "../../Headers/Entity.h"
#include "EntityAction.h"

#include <cmath>
#include <ncurses.h>
#include <numbers>

PlayerBehaviorController::PlayerBehaviorController(IInputHandler &input)
    : _inputHandler(input) {}

vec2f PlayerBehaviorController::Tick(Entity &self, float delta) {
  self.setShooting(false);

  vec2f moveDelta{0.f, 0.f};
  if (_inputHandler.keyDown(EntityAction::FORWARD)) {
    moveDelta += moveIntent(EntityAction::FORWARD, self, delta);
  }
  if (_inputHandler.keyDown(EntityAction::BACKWARD)) {
    moveDelta += moveIntent(EntityAction::BACKWARD, self, delta);
  }
  if (_inputHandler.keyDown(EntityAction::STRAFE_LEFT)) {
    moveDelta += moveIntent(EntityAction::STRAFE_LEFT, self, delta);
  }
  if (_inputHandler.keyDown(EntityAction::STRAFE_RIGHT)) {
    moveDelta += moveIntent(EntityAction::STRAFE_RIGHT, self, delta);
  }
  if (_inputHandler.keyDown(EntityAction::TURN_LEFT)) {
    moveDelta += moveIntent(EntityAction::TURN_LEFT, self, delta);
  }
  if (_inputHandler.keyDown(EntityAction::TURN_RIGHT)) {
    moveDelta += moveIntent(EntityAction::TURN_RIGHT, self, delta);
  }
  _cooldown -= delta;
  if (_inputHandler.keyDown(EntityAction::SHOOT) && _cooldown <= 0.f) {
    self.setShooting(true);
    _cooldown = _shootcd;
    if (_shootObserver)
      _shootObserver->onShoot();
  }
  return self.transform().position + moveDelta;
}

vec2f PlayerBehaviorController::moveIntent(EntityAction dir, Entity &self,
                                           float delta) {
  switch (dir) {
  case EntityAction::FORWARD:
    return {sinf(self.transform().angle) * self.moveSpeedOnDir(dir) * delta,
            cosf(self.transform().angle) * self.moveSpeedOnDir(dir) * delta};
    break;
  case EntityAction::BACKWARD:
    return {
        -1 * sinf(self.transform().angle) * self.moveSpeedOnDir(dir) * delta,
        -1 * cosf(self.transform().angle) * self.moveSpeedOnDir(dir) * delta};
    break;
  case EntityAction::STRAFE_LEFT:
    return {sinf(self.transform().angle - (std::numbers::pi_v<float> / 2.f)) *
                self.moveSpeedOnDir(dir) * delta,
            cosf(self.transform().angle - (std::numbers::pi_v<float> / 2.f)) *
                self.moveSpeedOnDir(dir) * delta};

    break;
  case EntityAction::STRAFE_RIGHT:
    return {sinf(self.transform().angle + (std::numbers::pi_v<float> / 2.f)) *
                self.moveSpeedOnDir(dir) * delta,
            cosf(self.transform().angle + (std::numbers::pi_v<float> / 2.f)) *
                self.moveSpeedOnDir(dir) * delta};

    break;
  case EntityAction::TURN_LEFT:
    self.addToAngle(-1 * self.moveSpeedOnDir(dir) * delta);
    return {0.f, 0.f};
    break;
  case EntityAction::TURN_RIGHT:
    self.addToAngle(self.moveSpeedOnDir(dir) * delta);
    return {0.f, 0.f};
    break;
  case EntityAction::SHOOT:
  case EntityAction::COUNT:
    // handled in Tick (cooldown + observer notification)
    return {0.f, 0.f};
    break;
  }
  return {0.f, 0.f};
}

void PlayerBehaviorController::onHit(Entity &self, Entity &other) {
  self.setHealth(self.health() - other.dmg());
  if (_healthObserver)
    _healthObserver->onHealthChanged(self.health());
}
