#include "../Headers/MapManager.h"
#include <execinfo.h>
#include <string>
#include <vector>

MapManager::MapManager() {} // so far nothing needs to be inited

MapManager::MapManager(const std::string &map, int mapWidth, int mapHeight)
    : _map(map), _mapWidth(mapWidth), _mapHeight(mapHeight) {}

void MapManager::uploadNewMap(const std::string &map, int newMapWidth,
                              int newMapHeight) {
  _map = map;
  _mapWidth = newMapWidth;
  _mapHeight = newMapHeight;
}

const std::string &MapManager::GetMap() const { return _map; }

int MapManager::mapHeight() const { return _mapHeight; }
int MapManager::mapWidth() const { return _mapWidth; }

bool MapManager::isWall(int test_x, int test_y) const {

  if (isOutOfBounds(test_x, test_y)) {
    return false;
  }
  if (_map[test_y * _mapWidth + test_x] == '#')
    return true;
  else
    return false;
}

bool MapManager::isMapAvailable() const {
  if (_map == "") {
    return false;
  } else {
    return true;
  }
}

const std::string &MapManager::getWallTexForMapChar(const char &mapChar) {
  return _wallTexMappers[mapChar].getTexture();
}

void MapManager::uploadWallTextureFor(const char &mapChar,
                                      std::string texture) {
  _wallTexMappers[mapChar] = TextureMapper(texture);
}

void MapManager::uploadWallTextureVecFor(const char &mapChar,
                                         std::vector<std::string> textureV) {
  _wallTexMappers[mapChar] = TextureMapper(textureV);
}

std::string MapManager::getWallTextureAt(int x, int y) {
  if (isOutOfBounds(x, y)) {
    return std::string();
  }
  char mapChar = _map[y * _mapWidth + x];
  return _wallTexMappers[mapChar].getTexture();
}

std::string MapManager::getWallTexColumnAt(int x, int y, int height,
                                           float hitpoint) {
  if (isOutOfBounds(x, y)) {
    return std::string();
  }
  char mapChar = _map[y * _mapWidth + x];
  return _wallTexMappers[mapChar].getTexColumnAt(height, hitpoint);
}

bool MapManager::isOutOfBounds(int test_x, int test_y) const {
  if (test_x < 0 || test_x >= _mapWidth || test_y < 0 || test_y >= _mapHeight) {
    return true;
  } else {
    return false;
  }
}
