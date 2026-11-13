#ifndef TQ_TEST_HELPERS_H
#define TQ_TEST_HELPERS_H

#include "Entity.h"
#include "ScriptedController.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct EntityOpts {
  int hp = 100;
  float collisionRadius = 0.2f;
  float hitRadius = 0.3f;
  bool isPlayer = false;
  float fovDeg = 90.f;
  float viewDistance = 10.f;
};

// Entity with a 1x1 texture ("x\n") and the given controller.
inline Entity makeEntity(vec2f pos, float angle,
                         std::unique_ptr<IBehaviorController> ctrl,
                         EntityOpts o = {}) {
  std::string tex = "x\n";
  return Entity(std::move(ctrl), Transform{pos, angle}, &tex, o.hp,
                o.collisionRadius, o.hitRadius, o.isPlayer, o.fovDeg,
                o.viewDistance);
}

// Entity driven and observed through a test-owned ControllerScript.
inline Entity makeEntity(vec2f pos, float angle, ControllerScript &script,
                         EntityOpts o = {}) {
  return makeEntity(pos, angle, std::make_unique<ScriptedController>(script),
                    o);
}

struct MapData {
  std::string map;
  int w;
  int h;
};

// Rows of equal length, e.g. {"#####", "#...#", "#####"}; '.' is floor.
inline MapData makeMap(const std::vector<std::string> &rows) {
  MapData m{"", static_cast<int>(rows.at(0).size()),
            static_cast<int>(rows.size())};
  for (const std::string &row : rows)
    m.map += row;
  return m;
}

#endif // TQ_TEST_HELPERS_H
