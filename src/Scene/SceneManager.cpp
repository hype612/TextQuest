#include "../Headers/SceneHandler.h"


SceneManager::SceneManager() 
{
  EngineState::_globalMapManager = &_mapManager;
  EngineState::_globalEntityManager = &_EntityManager;
  EngineState::_globalSceneManager = this;
}
SceneManager::SceneManager(std::wstring& map, int mapWidth, int mapHeight) // pass a map and construct a scene from that
{
  EngineState::_globalMapManager = &_mapManager;
  EngineState::_globalEntityManager = &_EntityManager;
  EngineState::_globalSceneManager = this;
  _mapManager.uploadNewMap(map, mapWidth, mapHeight);
}

void SceneManager::process()
{
  _entityManager.process();
  //_eventManager.process();
}


// Map Related functions
void SceneManager::initializeNewMap(std::wstring& map, int mapWidth, int mapHeight);

// Entity Related functions
void SceneManager::AddEntity(Entity& entity)
{
  _entityManager.AddEntity(entity);
}


void SceneManager::removeEntity(Entity& entity)
{
  _entityManager.removeEntity(entity);
}
void SceneManager::removeEntity(int entityId)
{
  _entityManager.removeEntity(entityId);
}
void SceneManager::removeAllEntities()
{
  _entityManager.removeAllEntities();
}






bool SceneManager::isOccupied(int x, int y) const {
  
}
