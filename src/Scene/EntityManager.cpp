#include "../Headers/EntityManager.h"
#include "../Headers/IEntitySceneChannel.h"
#include "../Headers/Logger.h"
#include "../Headers/TerrainCollidable.h"
#include "Entity.h"
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

EntityManager::EntityManager(IEntitySceneChannel &channel)
    : _sceneChannel(channel) {}

void EntityManager::process(float delta) {
  resolveStates();
  resolveMovement(delta);
}

void EntityManager::resolveStates() {
  for (Entity &e : _entityContainer) {
    if (e.health() <= 0) {
      e.setTransform({{-1.f, -1.f}, 0.f});
      e.setMoveSpeedAllDirectons(0.f);
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
        TerrainCollidable collided({static_cast<int>(current_trans.position.y),
                                    static_cast<int>(dest.y)});
        dest.y = current_trans.position.y;
        _entityContainer[id].onCollision(collided);
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
      }
      if (dist_newy - min_dist_sq < 0.f) {
        dest.y = current_trans.position.y;
        _entityContainer[id].onCollision(e);
      }
    }
  }
  // apply validated
  for (auto &[id, dest] : intents) {
    _entityContainer[id].setTransform(
        {dest, _entityContainer[id].transform().angle});
  }
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
