#include "../Headers/EntityManager.h"
#include "../Headers/IEntitySceneChannel.h"
#include "../Headers/Logger.h"
#include "../Headers/TerrainCollidable.h"
#include "Entity.h"
#include "EntityDistance.h"
#include "Transform.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

EntityManager::EntityManager(IEntitySceneChannel &channel)
    : _sceneChannel(channel) {}

void EntityManager::process(float delta) {
  resolveStates();
  resolveProjectiles();
  resolveMovement(delta);
  resolveVisibility();
}

void EntityManager::resolveStates() {
  for (Entity &e : _entityContainer) {
    if (e.health() <= 0) {
      e.setTransform({{-1.f, -1.f}, 0.f});
      e.setMoveSpeedAllDirectons(0.f);
    }
  }
}

void EntityManager::resolveProjectiles() {

  std::pair<EntityId, float> closest;
  for (Entity &e : _entityContainer) {
    if (!e.shooting()) {
      continue;
    }

    vec2f dir = {std::sin(e.transform().angle), std::cos(e.transform().angle)};
    closest = {-1, std::numeric_limits<float>::infinity()};
    for (Entity &other : _entityContainer) {
      if (e.ID() == other.ID())
        continue;

      if (auto d = projectileHitDistSq(e.transform().position,
                                       other.transform().position,
                                       other.collisionRadius(), dir, 20.f)) {
        if (*d < closest.second) {
          closest = {other.ID(), *d};
        }
      }
    }

    if (closest.first != -1) {
      _entityContainer[closest.first].onHit(e);
    }
  }
}

void EntityManager::resolveMovement(float delta) {
  // vec2f dest;
  std::vector<std::pair<EntityId, vec2f>> intents;
  intents.reserve(_entityContainer.size());

  for (Entity &e : _entityContainer) {
    intents.emplace_back(e.ID(), e.process(delta));
  }

  // validate intents
  for (auto &[id, dest] : intents) {
    Transform current_trans = _entityContainer[id].transform();
    if (dest != current_trans.position) {
      if (!_sceneChannel.canMoveTo({dest.x, current_trans.position.y})) {
        TerrainCollidable collided(
            {static_cast<int>(dest.x),
             static_cast<int>(current_trans.position.y)});
        dest.x = current_trans.position.x;
        _entityContainer[id].onCollision(collided);
      }
      if (!_sceneChannel.canMoveTo({current_trans.position.x, dest.y})) {
        TerrainCollidable collided({static_cast<int>(current_trans.position.x),
                                    static_cast<int>(dest.y)});
        dest.y = current_trans.position.y;
        _entityContainer[id].onCollision(collided);
      }
      // basically corner check
      if (!_sceneChannel.canMoveTo(dest)) {
        dest = current_trans.position;
      }
    }
    // its a wall pos --> quick exit
    if (dest == current_trans.position)
      continue;

    // Entity Check
    for (Entity &e : _entityContainer) {
      if (e.ID() == id)
        continue; // against itself guard
      Transform other_trans = e.transform();

      // do squared to save on std::sqrt time
      float min_dist =
          _entityContainer[id].collisionRadius() + e.collisionRadius();
      float min_dist_sq = min_dist * min_dist;
      float dist_newx = ((other_trans.position.x - dest.x) *
                         (other_trans.position.x - dest.x)) +
                        ((other_trans.position.y - current_trans.position.y) *
                         (other_trans.position.y - current_trans.position.y));
      float dist_newy = ((other_trans.position.x - current_trans.position.x) *
                         (other_trans.position.x - current_trans.position.x)) +
                        ((other_trans.position.y - dest.y) *
                         (other_trans.position.y - dest.y));
      if (dist_newx - min_dist_sq < 0.f) {
        dest.x = current_trans.position.x;
        _entityContainer[id].onCollision(e);
        e.onCollision(_entityContainer[id]);
      }
      if (dist_newy - min_dist_sq < 0.f) {
        dest.y = current_trans.position.y;
        _entityContainer[id].onCollision(e);
        e.onCollision(_entityContainer[id]);
      }
    }
  }
  // apply validated
  for (auto &[id, dest] : intents) {
    _entityContainer[id].setTransform(
        {dest, _entityContainer[id].transform().angle});
  }
}

void EntityManager::resolveVisibility() {
  // on each non_player: check dist
  for (Entity &e : _entityContainer) {
    if (e.isPlayer())
      continue;
    Transform current_trans = e.transform();
    for (Entity &other : _entityContainer) {
      // self check
      if (e.ID() == other.ID())
        continue;
      Transform other_trans = other.transform();
      float dist_sq = ((other_trans.position.x - current_trans.position.x) *
                       (other_trans.position.x - current_trans.position.x)) +
                      ((other_trans.position.y - current_trans.position.y) *
                       (other_trans.position.y - current_trans.position.y));
      // distance check
      if (dist_sq - e.viewDistance() * e.viewDistance() > 0.f)
        continue;

      vec2f relative_pos = other_trans.position - current_trans.position;
      vec2f focus_point = {sinf(current_trans.angle),
                           cosf(current_trans.angle)};
      float diff_cos = (focus_point.dot(relative_pos)) /
                       (std::sqrt((focus_point.x * focus_point.x +
                                   focus_point.y * focus_point.y) *
                                  (relative_pos.x * relative_pos.x +
                                   relative_pos.y * relative_pos.y)));

      diff_cos = std::clamp(diff_cos, -1.f, 1.f);
      float rad_diff = acosf(diff_cos);

      // in fov check
      if (rad_diff > e.fov() / 2.f)
        continue;

      // obstruction check w/ raycast
      float distToTarget = std::sqrt(dist_sq);

      bool hitwall = false;
      float rayLength = 0;
      vec2f rayDir = relative_pos.normalized();
      vec2f deltaDist{(rayDir.x == 0.f) ? 1e30f : std::abs(1.f / rayDir.x),
                      (rayDir.y == 0.f) ? 1e30f : std::abs(1.f / rayDir.y)};

      vec2i stepDir{(rayDir.x >= 0.f) ? 1 : -1, (rayDir.y >= 0.f) ? 1 : -1};
      vec2f mapPos{std::floor(current_trans.position.x),
                   std::floor(current_trans.position.y)};
      vec2f sideDist{
          (stepDir.x == 1) ? mapPos.x + 1.f - current_trans.position.x
                           : current_trans.position.x - mapPos.x,
          (stepDir.y == 1) ? mapPos.y + 1.f - current_trans.position.y
                           : current_trans.position.y - mapPos.y};
      if (sideDist.x <= 0.0001f)
        sideDist.x = 1.f;
      if (sideDist.y <= 0.0001f)
        sideDist.y = 1.f;
      sideDist.x = sideDist.x * deltaDist.x;
      sideDist.y = sideDist.y * deltaDist.y;
      while (!hitwall && rayLength < distToTarget) {
        if (sideDist.x < sideDist.y) {
          sideDist.x += deltaDist.x;
          mapPos.x += stepDir.x;
          rayLength = sideDist.x;
        } else {
          sideDist.y += deltaDist.y;
          mapPos.y += stepDir.y;
          rayLength = sideDist.y;
        }

        if (!_sceneChannel.canMoveTo({mapPos.x, mapPos.y})) {
          hitwall = true;
          Logger::GetInstance()->log(
              "hitwall at mapPos: " + std::to_string(mapPos.x) + "," +
                  std::to_string(mapPos.y) +
                  " istvan pos: " + std::to_string(current_trans.position.x) +
                  "," + std::to_string(current_trans.position.y) +
                  " player pos: " + std::to_string(other_trans.position.x) +
                  "," + std::to_string(other_trans.position.y),
              LogType::CORE, LogLevel::INFO);
        }
      }

      if (hitwall)
        continue;

      // got through all checks, entity is visible
      Logger::GetInstance()->log(
          "istvan sees you at position: " +
              std::to_string(other.transform().position.x) + ":" +
              std::to_string(other.transform().position.y),
          LogType::CORE, LogLevel::INFO);
      e.onVisible(other);
    }
  }
}

std::optional<float>
EntityManager::projectileHitDistSq(vec2f origin, vec2f target, float hit_delta,
                                   vec2f dir, float max_dist) {
  if (dir.x * dir.x + dir.y * dir.y - 1.f >= 0.0001f) {
    return std::nullopt;
  }
  vec2f to_delta = target - origin;
  float cross = to_delta.cross(dir);

  if (std::abs(cross) > hit_delta) {
    return std::nullopt;
  }

  float dot = to_delta.dot(dir);

  if (dot < -hit_delta || dot > max_dist + hit_delta) {
    return std::nullopt;
  }

  vec2f hit_point = origin + dir * dot;
  vec2f diff = hit_point - origin;
  return diff.x * diff.x + diff.y * diff.y;
}

// ================================
// Container Getters
// ================================

int EntityManager::getEntityIdAtPos(int coord_x, int coord_y) const {
  for (const Entity &e : _entityContainer) {
    if (e.transform().position.x == coord_x &&
        e.transform().position.y == coord_y)
      return e.ID();
  }

  return -1;
}

int EntityManager::getEntityCount() const { return _entityContainer.size(); }

// ================================
// Container Setters
// ================================

void EntityManager::addEntity(Entity &entity) {
  if (entity.ID() != -1) {
    std::cerr << "id has been altered. Please do not touch id" << std::endl;
    return;
  }
  entity.setID(_entityContainer.size());
  _entityContainer.push_back(std::move(entity));
}
void EntityManager::removeEntity(int id) {
  _entityContainer[id].setTransform({{-1, -1}, 0.f});
}

Entity &EntityManager::entityAtId(int id) { return _entityContainer[id]; }

void EntityManager::removeAllEntities() {
  _entityContainer.clear();
  _entityContainer.shrink_to_fit();
}

// ================================
// for renderer
// ================================

std::string EntityManager::getCurrentEntityTexture(int EntityId) const {
  return _entityContainer[EntityId].getTexture();
}

std::string EntityManager::getCurrentEntityTexture(const Entity &entity) const {
  return entity.getTexture();
}

const std::vector<int> &
EntityManager::getCurrentEntityTexMask(int EntityId) const {
  return _entityContainer[EntityId].getTexMask();
}

const std::vector<int> &
EntityManager::getCurrentEntityTexMask(const Entity &entity) const {
  return entity.getTexMask();
}
std::string EntityManager::scaledTexOfEntity(int entityId, unsigned int width,
                                             unsigned int height,
                                             int shadingIdx) const {
  if (entityId < 0 ||
      static_cast<unsigned int>(entityId) > _entityContainer.size()) {
    Logger::GetInstance()->log("Entity outside of container size.",
                               LogType::RENDER, LogLevel::ERROR);
    return std::string();
  }
  return _entityContainer[entityId].scaledTex(width, height, shadingIdx);
}
std::vector<int> EntityManager::scaledMaskOfEntity(int entityId,
                                                   unsigned int width,
                                                   unsigned int height) const {
  if (entityId < 0 ||
      static_cast<unsigned int>(entityId) > _entityContainer.size()) {
    Logger::GetInstance()->log("Entity outside of container size.",
                               LogType::RENDER, LogLevel::ERROR);
    return std::vector<int>();
  }
  return _entityContainer[entityId].scaledMask(width, height);
}
std::vector<EntityDistance>
EntityManager::entitiesSortedByDistanceTo(const vec2f &target) const {
  std::vector<EntityDistance> r;
  r.reserve(_entityContainer.size());
  for (const Entity &e : _entityContainer) {
    float dist = std::sqrt(((e.transform().position.x - target.x) *
                            (e.transform().position.x - target.x)) +
                           ((e.transform().position.y - target.y) *
                            (e.transform().position.y - target.y)));
    r.push_back({e.ID(), dist});
  }
  std::sort(r.begin(), r.end());
  return r;
}
