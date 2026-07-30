#include "../Headers/IstvanBehaviorController.h"
#include "../Headers/Entity.h"

vec2f IstvanBehaviorController::Tick(Entity &self, float delta) {
  return self.transform().position;
}
