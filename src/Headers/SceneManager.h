#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "./EngineState.h"
#include "./Entity.h"
#include "./EntityManager.h"
#include "./Logger.h"
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
  void initializeNewMap(std::string &map, int mapWidth, int mapHeight);
  int getMapHeight() const;
  int getMapWidth() const;
  bool isMapAvailable() const;
  void uploadTextureForWall(const char &mapChar, std::string &wallTex);
  void uploadTextureVecForWall(const char &mapChar,
                               std::vector<std::string> &wallTexV);
  const std::string &getWallTextureForMapChar(const char &mapChar);

  // Entity Related functions
  void AddEntity(Entity &entity);
  void removeEntity(Entity &entity);
  void removeEntity(int entityId);
  void removeAllEntities();
  void uploadTextureForEntity(std::string &entityTex);
  void uploadTextureVecForEntity(std::vector<std::string> &entityTex);

  // Player
  void setPlayerX(int new_x);
  void setPlayerY(int new_y);
  Player &getPlayerRef();

  // Other
  void loadResources(const std::string &filePath,
                     std::unordered_map<std::string, std::string> &outTextures);

private:
  EntityManager &_entityManager;
  MapManager &_mapManager;
  Player _player;
};

#endif // SCENEMANAGER_H
