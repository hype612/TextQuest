#include "../Headers/SceneManager.h"

SceneManager::SceneManager() { EngineState::_globalSceneManager = this; }
SceneManager::SceneManager(
    std::wstring &map, int mapWidth,
    int mapHeight) // pass a map and construct a scene from that
{
  EngineState::_globalSceneManager = this;
  _mapManager.uploadNewMap(map, mapWidth, mapHeight);
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

void SceneManager::rescaleTextureOf(int pos_x, int pos_y, float distance) {
  if (_mapManager.isWall(pos_x, pos_y) == true)
    _mapManager.rescaleWallTextureAt(pos_x, pos_y);
  int e_id = _entityManager.getEntityIdAtPos(pos_x, pos_y);
  if (e_id != -1)
    _entityManager.rescaleEntityTexture(e_id, distance);
  else
    std::cerr << "invalid position" << std::endl;
}

std::wstring SceneManager::getTextureAt(int pos_x, int pos_y) {
  if (_mapManager.isWall() == true)
    _mapManager.getWallTextureAt(pos_x, pos_y);
  int e_id = _entityManager.getEntityIdAtPos(pos_x, pos_y);
  if (e_id != -1)
    _entityManager.getCurrentEntityTexture(e_id);
  else
    std::cerr << "invalid position" << std::endl;
}

std::wstring getNextCharColumnAt(int pos_x, int pos_y) {
  if (_mapManager.isWall() == true)
    _mapManager.getWallTexColumnAt(pos_x, pos_y);
  int e_id = _entityManager.getEntityIdAtPos(pos_x, pos_y);
  if (e_id != -1)
    _entityManager.getNextEntityCharColumn(e_id);
  else
    std::cerr << "invalid position" << std::endl;
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

bool SceneManager::isOccupied(int x, int y) const {
  if (_entityManager.getEntityIdAtPos(x, y) != -1)
    return true;

  if (_mapManager.isWall(x, y) == true)
    return true;

  return false;
}
