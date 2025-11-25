#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "./EngineState.h"
#include "./Entity.h"
#include "./EntityManager.h"
#include "./MapManager.h"
#include "./Tile.h"
#include "./player.h"
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

class SceneManager {
public:
  SceneManager(EntityManager &entityMan, MapManager &mapMan);

  void process();
  Tile isOccupied(int x, int y) const;

  // Map Related functions
  void initializeNewMap(std::wstring &map, int mapWidth, int mapHeight);
  int getMapHeight() const;
  int getMapWidth() const;
  bool isMapAvailable() const;
  void uploadTextureForWall(wchar_t mapChar, std::wstring &wallTex);

  // Entity Related functions
  void AddEntity(Entity &entity);
  void removeEntity(Entity &entity);
  void removeEntity(int entityId);
  void removeAllEntities();
  void uploadTextureForEntity(std::wstring &entityTex);

  // Player
  void setPlayerX(int new_x);
  void setPlayerY(int new_y);
  Player &getPlayerRef();

  // Other
  std::unordered_map<std::string, std::wstring>
  loadResources(const std::string &filePath);

private:
  EntityManager &_entityManager;
  MapManager &_mapManager;
  Player _player;
};

#endif // SCENEMANAGER_H
