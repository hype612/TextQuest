#include "../Headers/IstvanBehaviorController.h"
#include "../Headers/Entity.h"
#include "../Headers/Logger.h"
#include "ICollidable.h"
#include "MoveDirection.h"
#include <string>

IstvanBehaviorController::IstvanBehaviorController() {
  //_alternates[0] = MoveDirection::STRAFE_LEFT;
  //_alternates[1] = MoveDirection::STRAFE_RIGHT;
  //_a_idx = 0;
}

vec2f IstvanBehaviorController::Tick(Entity &self, float delta) {

  float dist_to_target_sq = (_target.x - self.transform().position.x) *
                                (_target.x - self.transform().position.x) +
                            (_target.y - self.transform().position.y) *
                                (_target.y - self.transform().position.y);

  float angle_diff = _targetAngle - self.transform().angle;
  angle_diff = std::atan2(std::sin(angle_diff), std::cos(angle_diff));
  float angle_epsilon = 0.05f;
  vec2f ret = self.transform().position;
  Logger::GetInstance()->log("Istvan angle: " +
                                 std::to_string(self.transform().angle),
                             LogType::CORE, LogLevel::INFO);
  if (angle_diff > angle_epsilon) {
    ret += moveIntent(MoveDirection::TURN_RIGHT, self, delta);
  }

  if (angle_diff < -angle_epsilon) {
    ret += moveIntent(MoveDirection::TURN_LEFT, self, delta);
  }

  if (dist_to_target_sq >= .3f)
    ret += moveIntent(MoveDirection::FORWARD, self, delta);
  return ret;
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
  _a_idx = (_a_idx + 1) % 2;
  if (other.collidableKind() == Collidable::ENTITY)
    self.setHealth(self.health() - 10);
}
void IstvanBehaviorController::onVisible(Entity &self, Entity &other) {
  // turn to the player
  if (!other.isPlayer())
    return;

  vec2f relative_pos = other.transform().position - self.transform().position;
  _targetAngle = std::atan2(relative_pos.x, relative_pos.y);
  _target = other.transform().position;
  self.setMoveSpeedAllDirectons(1.5f);
  self.setTurnSpeedAlldirections(3.f);
}

void IstvanBehaviorController::onHit(Entity &self, Entity &other) {
  Logger::GetInstance()->log("istvan was shot", LogType::SCENE,
                             LogLevel::WARNING);
  self.setHealth(self.health() - other.dmg());
}
