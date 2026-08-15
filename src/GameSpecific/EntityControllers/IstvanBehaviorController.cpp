#include "../Headers/IstvanBehaviorController.h"
#include "../Headers/Entity.h"
#include "ICollidable.h"
#include "MoveDirection.h"

IstvanBehaviorController::IstvanBehaviorController() {
  _alternates[0] = MoveDirection::STRAFE_LEFT;
  _alternates[1] = MoveDirection::STRAFE_RIGHT;
  _a_idx = 0;
}

vec2f IstvanBehaviorController::Tick(Entity &self, float delta) {
  // self.transform().position;
  return self.transform().position +
         moveIntent(_alternates[_a_idx], self, delta);
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
  }
  return {0.f, 0.f};
}

void IstvanBehaviorController::onCollision(Entity &self, ICollidable &) {
  _a_idx = (_a_idx + 1) % 2;
  self.setHealth(self.health() - 10);
}
