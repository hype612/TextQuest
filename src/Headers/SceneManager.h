#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "./EngineState.h"
#include "./EntityManager.h"
#include "./MapManager.h"
#include "Entity.h"
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class SceneManager {
public:
  SceneManager(
      EntityManager &entityMan,
      MapManager &mapMan); // pass a map and construct a scene from that

  void process();
  bool isOccupied(int x, int y) const;

  // Map Related functions
  void initializeNewMap(std::wstring &map, int mapWidth, int mapHeight);
  int getMapHeight() const;
  int getMapWidth() const;
  bool isMapAvailable() const;

  // Entity Related functions
  void AddEntity(Entity &entity);
  void removeEntity(Entity &entity);
  void removeEntity(int entityId);
  void removeAllEntities();

private:
  // std::unordered_map<coord_t, int, coord_hash> _entityPositionMap; // stores
  // entity._id, indexes with coordinates
  MapManager &_mapManager;
  EntityManager &_entityManager;
};

#endif // SCENEMANAGER_H
