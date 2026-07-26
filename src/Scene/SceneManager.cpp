#include "../Headers/SceneManager.h"
#include "./Camera.h"
#include "./MapManager.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

SceneManager::SceneManager() : _entityManager(*this), _mapManager() {}
void SceneManager::process(float delta) {
  _entityManager.process(delta);
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

bool SceneManager::canMoveTo(const vec2f &dest) const {
  return !_mapManager.isWall(static_cast<int>(dest.x),
                             static_cast<int>(dest.y)) &&
         !_mapManager.isOutOfBounds(static_cast<int>(dest.x),
                                    static_cast<int>(dest.y));
}

bool SceneManager::isOutOfBounds(int test_x, int test_y) const {
  return _mapManager.isOutOfBounds(test_x, test_y);
}
bool SceneManager::isWall(int test_x, int test_y) const {
  return _mapManager.isWall(test_x, test_y);
}

std::string SceneManager::getWallTexColumnAt(int x, int y, int height,
                                             float hitpoint, int visibleTop,
                                             int visibleBot) const {
  return _mapManager.getWallTexColumnAt(x, y, height, hitpoint, visibleTop,
                                        visibleBot);
}

std::string SceneManager::getWallTexColumnAt(int x, int y, int height,
                                             float hitpoint, int visibleTop,
                                             int visibleBot,
                                             int shadingIdx) const {
  return _mapManager.getWallTexColumnAt(x, y, height, hitpoint, visibleTop,
                                        visibleBot, shadingIdx);
}

// Entity Related functions
Entity &SceneManager::entityAtId(int id) {
  return _entityManager.entityAtId(id);
}
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

// Camera
const Camera &SceneManager::camera() const {
  if (_camera.has_value()) {
    std::cerr << "Camera accessed before SetCameraFollow was ever called"
              << std::endl;
    std::exit(EXIT_FAILURE);
  }
  return *_camera;
}

const Camera *SceneManager::cameraPtr() const {
  return _camera.has_value() ? &(*_camera) : nullptr;
}

const Transform &SceneManager::cameraFollow() const {
  if (_camera.has_value()) {
    std::cerr << "Camera accessed before SetCameraFollow was ever called"
              << std::endl;
    std::exit(EXIT_FAILURE);
  }
  return _camera->follow();
}

void SceneManager::setCameraFollow(const Transform &new_follow) {
  if (!_camera.has_value())
    _camera.emplace(new_follow);
  else
    _camera->setFollow(new_follow);
}

void SceneManager::setCameraFovDegrees(float newfov) {
  _camera->setFovDegrees(newfov);
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
