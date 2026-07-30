#include "../Headers/EntityManager.h"
#include "../Headers/IEntitySceneChannel.h"
#include "../Headers/Logger.h"
#include "Entity.h"
#include <cmath>
#include <iostream>
#include <vector>

EntityManager::EntityManager(IEntitySceneChannel &channel)
    : _sceneChannel(channel) {}

void EntityManager::process(float delta) {
  vec2f dest;
  for (Entity &e : _entityContainer) {
    dest = e.process(delta);
    if (dest != e.transform().position) {
      if (_sceneChannel.canMoveTo({dest.x, e.transform().position.y})) {
        e.setTransform(
            {{dest.x, e.transform().position.y}, e.transform().angle});
      }
      if (_sceneChannel.canMoveTo({e.transform().position.x, dest.y})) {
        e.setTransform(
            {{e.transform().position.x, dest.y}, e.transform().angle});
      }
    }
  }
}

// ================================
// Container Getters
// ================================

// naive searches, later add something to speed it up if neccessary
std::optional<std::reference_wrapper<Entity>>
EntityManager::getEntityAtPos(int coord_x, int coord_y) {
  for (Entity &e : _entityContainer) {
    if (e.transform().position.x == coord_x &&
        e.transform().position.y == coord_y)
      return e;
  }
  return std::nullopt;
}

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

void EntityManager::removeEntity(int coord_x, int coord_y) {
  int id = getEntityIdAtPos(coord_x, coord_y);
  removeEntity(id);
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
