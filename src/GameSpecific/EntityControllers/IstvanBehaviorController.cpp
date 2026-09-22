#include "../Headers/IstvanBehaviorController.h"
#include "../Headers/Entity.h"
#include "ICollidable.h"
#include "MoveDirection.h"
#include "Vec2f.h"
#include <cmath>
#include <cstdlib>
#include <numbers>
#include <utility>

IstvanBehaviorController::IstvanBehaviorController(
    std::vector<vec2f> patrol_points)
    : _state(State::IDLE), _patrolPoints(std::move(patrol_points)),
      _targetE(nullptr) {}

vec2f IstvanBehaviorController::Tick(Entity &self, float delta) {
  self.setShooting(false);
  _lastShot += delta;
  updateState(self);
  vec2f ret = self.transform().position;
  switch (_state) {
  case State::IDLE:
    patrol(self, ret, delta);
    break;
  case State::CHASE:
    chase(self, ret, delta);
    break;
  case State::ATTACK:
    shoot(self, delta);
    break;
  }
  return ret;
}

void IstvanBehaviorController::patrol(Entity &self, vec2f &acc_intent,
                                      float delta) {
  if (_patrolPoints.size() < 2)
    return;

  vec2f rel = _patrolPoints[_patrolIdx] - self.transform().position;
  if (rel.x * rel.x + rel.y * rel.y <= _patrolArriveDistSq) {
    int next = _patrolIdx + _patrolDir;
    if (next < 0 || next >= static_cast<int>(_patrolPoints.size())) {
      _patrolDir = -_patrolDir;
      next = _patrolIdx + _patrolDir;
    }
    _patrolIdx = next;
    return;
  }

  float angle_diff = std::atan2(rel.x, rel.y) - self.transform().angle;
  angle_diff = std::atan2(std::sin(angle_diff), std::cos(angle_diff));
  const float angle_epsilon = 0.05f;
  if (angle_diff > angle_epsilon)
    acc_intent += moveIntent(MoveDirection::TURN_RIGHT, self, delta);
  else if (angle_diff < -angle_epsilon)
    acc_intent += moveIntent(MoveDirection::TURN_LEFT, self, delta);

  if (std::abs(angle_diff) < _patrolFaceTolerance)
    acc_intent += moveIntent(MoveDirection::FORWARD, self, delta);
}
void IstvanBehaviorController::chase(Entity &self, vec2f &acc_intent,
                                     float delta) {
  vec2f target = _targetE->transform().position;
  float dist_to_target_sq = (target.x - self.transform().position.x) *
                                (target.x - self.transform().position.x) +
                            (target.y - self.transform().position.y) *
                                (target.y - self.transform().position.y);

  float angle_diff = _targetAngle - self.transform().angle;
  angle_diff = std::atan2(std::sin(angle_diff), std::cos(angle_diff));
  float angle_epsilon = 0.05f;
  if (angle_diff > angle_epsilon) {
    acc_intent += moveIntent(MoveDirection::TURN_RIGHT, self, delta);
  }

  if (angle_diff < -angle_epsilon) {
    acc_intent += moveIntent(MoveDirection::TURN_LEFT, self, delta);
  }

  if (dist_to_target_sq > _maxShootDist * _maxShootDist)
    acc_intent += moveIntent(MoveDirection::FORWARD, self, delta);
}

void IstvanBehaviorController::shoot(Entity &self, float delta) {
  _lastShot += delta;
  if (_lastShot < _shootCooldown) {
    return;
  }
  bool shootstate = (rand() % 100) <= _misschance;
  self.setShooting(shootstate);
  _lastShot = 0.f;
}

// Only change the state
// do not introduce side effects
void IstvanBehaviorController::updateState(Entity &self) {
  if (_targetE == nullptr) {
    return;
  }

  vec2f target = _targetE->transform().position;
  float dist_to_target_sq = (target.x - self.transform().position.x) *
                                (target.x - self.transform().position.x) +
                            (target.y - self.transform().position.y) *
                                (target.y - self.transform().position.y);

  if (_state == State::IDLE) {
    if (dist_to_target_sq > _maxShootDist * _maxShootDist) {
      _state = State::CHASE;
      return;
    } else {
      _state = State::ATTACK;
    }
  } else if (_state == State::CHASE) {

    if (dist_to_target_sq <= _maxShootDist * _maxShootDist) {
      _state = State::ATTACK;
    }
  } else if (_state == State::ATTACK) {

    if (dist_to_target_sq > _maxShootDist * _maxShootDist) {
      _state = State::CHASE;
    }
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
  // do nothing for now
}
void IstvanBehaviorController::onVisible(Entity &self, Entity &other) {
  // turn to the player
  if (!other.isPlayer() || _targetE != nullptr) {
    return;
  }

  _targetE = &other;
  // vec2f relative_pos = other.transform().position -
  // self.transform().position; _targetAngle = std::atan2(relative_pos.x,
  // relative_pos.y); _target = other.transform().position;
}

void IstvanBehaviorController::onHit(Entity &self, Entity &other) {
  self.setHealth(self.health() - other.dmg());
}
