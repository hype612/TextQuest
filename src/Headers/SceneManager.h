#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "./EngineState.h"
#include "./EntityManager.h"
#include "./MapManager.h"
#include "./Tile.h"
#include "./player.h"
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
  Tile isOccupied(int x, int y) const;

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

  // Player
  void setPlayerX(int new_x);
  void setPlayerY(int new_y);
  Player &getPlayerRef();

private:
  MapManager &_mapManager;
  EntityManager &_entityManager;
  Player _player;
};

#endif // SCENEMANAGER_H
