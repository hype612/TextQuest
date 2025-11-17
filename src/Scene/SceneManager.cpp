#include "../Headers/SceneManager.h"

SceneManager::SceneManager(EntityManager &entityMan, MapManager &mapMan)
    : _entityManager(entityMan),
      _mapManager(mapMan) // pass a map and construct a scene from that
{
  EngineState::_globalSceneManager = this;
}

void SceneManager::process() {
  _entityManager.process();
  //_eventManager.process();
}

// Map Related functions
void SceneManager::initializeNewMap(std::wstring &map, int mapWidth,
                                    int mapHeight) {
  _mapManager.uploadNewMap(map, mapWidth, mapHeight);
}

int SceneManager::getMapHeight() const { return _mapManager.mapHeight(); }
int SceneManager::getMapWidth() const { return _mapManager.mapWidth(); }

bool SceneManager::isMapAvailable() const {
  return _mapManager.isMapAvailable();
}
// Entity Related functions
void SceneManager::AddEntity(Entity &entity) {
  _entityManager.addEntity(entity);
}

void SceneManager::removeEntity(Entity &entity) {
  _entityManager.removeEntity(entity.ID());
}
void SceneManager::removeEntity(int entityId) {
  _entityManager.removeEntity(entityId);
}
void SceneManager::removeAllEntities() { _entityManager.removeAllEntities(); }

Tile SceneManager::isOccupied(int x, int y) const {
  if (_entityManager.getEntityIdAtPos(x, y) != -1)
    return Tile::ENTITY;

  if (_mapManager.isWall(x, y) == true)
    return Tile::WALL;

  return Tile::NONE;
}
