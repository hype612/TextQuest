#include "../Headers/IstvanBehaviorController.h"
#include "../Headers/Entity.h"
#include "ICollidable.h"
#include "MoveDirection.h"

IstvanBehaviorController::IstvanBehaviorController() : _state(State::IDLE) {}

vec2f IstvanBehaviorController::Tick(Entity &self, float delta) {
  self.setShooting(false);
  _targetLastSeen += delta;
  _lastShot += delta;
  updateState(self);
  /*
  float dist_to_target_sq = (_target.x - self.transform().position.x) *
                                (_target.x - self.transform().position.x) +
                            (_target.y - self.transform().position.y) *
                                (_target.y - self.transform().position.y);

  float angle_diff = _targetAngle - self.transform().angle;
  angle_diff = std::atan2(std::sin(angle_diff), std::cos(angle_diff));
  float angle_epsilon = 0.05f;
  vec2f ret = self.transform().position;
  if (angle_diff > angle_epsilon) {
    ret += moveIntent(MoveDirection::TURN_RIGHT, self, delta);
  }

  if (angle_diff < -angle_epsilon) {
    ret += moveIntent(MoveDirection::TURN_LEFT, self, delta);
  }

  if (dist_to_target_sq >= .3f)
    ret += moveIntent(MoveDirection::FORWARD, self, delta);
  */
  return ret;
}

void IstvanBehaviorController::updateState(Entity &self) {
  if (_state == State::IDLE) {
    // check if we see the player
    if (!_hasTarget) {
      return;
    }
    // has target - check if we can shoot
    if (_targetAngle < 10.f) {
    }
  } else if (_state == State::CHASE) {
    // Check if close enough to shoot
  } else if (_state == State::ATTACK) {
    // Check if too far away to shoot
  }
}

vec2f IstvanBehaviorController::moveIntent(MoveDirection dir, Entity &self,
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
    return {
        std::sin(self.transform().angle + (std::numbers::pi_v<float> / 2.f)) *
            self.moveSpeedOnDir(dir) * delta,
        std::cos(self.transform().angle + (std::numbers::pi_v<float> / 2.f)) *
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
  }
  return {0.f, 0.f};
}

void IstvanBehaviorController::onCollision(Entity &self, ICollidable &other) {
  if (other.collidableKind() == Collidable::ENTITY)
    self.setHealth(self.health() - 10);
}
void IstvanBehaviorController::onVisible(Entity &self, Entity &other) {
  // turn to the player
  if (!other.isPlayer() || _hasTarget) {
    return;
  }

  _targetE = &other;
  _hasTarget = true;
  // vec2f relative_pos = other.transform().position -
  // self.transform().position; _targetAngle = std::atan2(relative_pos.x,
  // relative_pos.y); _target = other.transform().position;
}

void IstvanBehaviorController::onHit(Entity &self, Entity &other) {
  self.setHealth(self.health() - other.dmg());
}
