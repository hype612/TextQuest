#include "../Headers/SceneManager.h"
#include "./Camera.h"
#include "./MapManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

SceneManager::SceneManager(EntityManager &entityMan, MapManager &mapMan)
    : _entityManager(entityMan), _mapManager(mapMan),
      _player(-1, -1, 0.f, mapMan), _camera(_player.transform()) {
  _camera.setFovDegrees(90);
}
// 1.0472
void SceneManager::process() {
  _entityManager.process();
  //_eventManager.process();
}

// Map Related functions
void SceneManager::initializeNewMap(std::string &map, int mapWidth,
                                    int mapHeight) {
  _mapManager.uploadNewMap(map, mapWidth, mapHeight);
}

int SceneManager::getMapHeight() const { return _mapManager.mapHeight(); }
int SceneManager::getMapWidth() const { return _mapManager.mapWidth(); }

bool SceneManager::isMapAvailable() const {
  return _mapManager.isMapAvailable();
}

void SceneManager::uploadTextureForWall(const char &mapChar,
                                        std::string &wallTex) {
  _mapManager.uploadWallTextureFor(mapChar, wallTex);
}

void SceneManager::uploadTextureVecForWall(const char &mapChar,
                                           std::vector<std::string> &wallTexV) {
  _mapManager.uploadWallTextureVecFor(mapChar, wallTexV);
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

// Player related functions
void SceneManager::setPlayerX(int new_x) { _player.setX(new_x); }
void SceneManager::setPlayerY(int new_y) { _player.setY(new_y); }
Player &SceneManager::getPlayerRef() { return _player; }

// Camera
const Camera &SceneManager::camera() const { return _camera; }
const Camera *SceneManager::cameraPtr() const { return &_camera; }
const Transform &SceneManager::cameraFollow() const { return _camera.follow(); }
void SceneManager::setCameraFollow(const Transform &new_follow) {
  _camera.setFollow(new_follow);
}

// Other
void SceneManager::loadResources(
    const std::string &filePath,
    std::unordered_map<std::string, std::string> &outTextures) {
  std::stringstream temp;
  std::string value;
  std::string key;
  for (const auto &entry : std::filesystem::directory_iterator(filePath)) {
    if (std::filesystem::is_regular_file(entry)) {
      std::ifstream current_file(entry.path());
      if (current_file.fail()) {
        std::cerr << "ERROR: failed to open file: " << entry.path().string()
                  << " reading next texture file..." << std::endl;
        continue;
      }

      temp.str(std::string());
      temp << current_file.rdbuf();
      key = entry.path().stem().string();
      if (outTextures.find(key) != outTextures.end()) {
        std::cerr << "ERROR: texture for this type of tile is already present. "
                     "Skipping this one...."
                  << std::endl;
        std::cerr << "texture: " << std::endl;
        std::cout << temp.str() << std::endl;
        continue;
      }
      value = temp.str();
      outTextures[key] = value;
      current_file.close();
    }
  }
}
