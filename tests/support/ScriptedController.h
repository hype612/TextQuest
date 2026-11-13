#ifndef TQ_TEST_SCRIPTEDCONTROLLER_H
#define TQ_TEST_SCRIPTEDCONTROLLER_H

#include "Entity.h"
#include "IBehaviorController.h"
#include <deque>
#include <vector>

struct ControllerScript {
  // Each Tick pops one intent; with none queued the entity stays where it is.
  std::deque<vec2f> intents;
  // Everything the controller was told, in call order.
  std::vector<Collidable> collisions;
  std::vector<int> visibleIds; // ID of the entity that was seen
  std::vector<int> hitByIds;   // ID of the entity that hit us
};

class ScriptedController : public IBehaviorController {
public:
  explicit ScriptedController(ControllerScript &script) : _script(script) {}

  vec2f Tick(Entity &self, float) override {
    if (_script.intents.empty())
      return self.transform().position;
    vec2f next = _script.intents.front();
    _script.intents.pop_front();
    return next;
  }
  void onCollision(Entity &, ICollidable &other) override {
    _script.collisions.push_back(other.collidableKind());
  }
  void onVisible(Entity &, Entity &other) override {
    _script.visibleIds.push_back(other.ID());
  }
  void onHit(Entity &, Entity &other) override {
    _script.hitByIds.push_back(other.ID());
  }

private:
  ControllerScript &_script;
};

#endif // TQ_TEST_SCRIPTEDCONTROLLER_H
