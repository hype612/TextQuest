#include "../Headers/MapManager.h"

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

void MapManager::uploadWallTextureFor(const char &mapChar,
                                      std::string texture) {
  _wallTexMappers[mapChar] = TextureMapper(texture);
}

void MapManager::rescaleWallTextureAt(int x, int y, float distance) {
  if (isOutOfBounds(x, y)) {
    return;
  }
  char mapChar = _map[y * _mapWidth + x];
  _wallTexMappers[mapChar].rescaleCurrentTexture(distance);
}

std::string MapManager::getWallTextureAt(int x, int y) {
  if (isOutOfBounds(x, y)) {
    return std::string();
  }
  char mapChar = _map[y * _mapWidth + x];
  return _wallTexMappers[mapChar].getTexture();
}

std::string MapManager::getWallTexColumnAt(int x, int y, int height) {
  if (isOutOfBounds(x, y)) {
    return std::string();
  }
  char mapChar = _map[y * _mapWidth + x];
  return _wallTexMappers[mapChar].getNextTexColumn(height);
}

bool MapManager::isOutOfBounds(int test_x, int test_y) const {
  if (test_x < 0 || test_x >= _mapWidth || test_y < 0 || test_y >= _mapHeight) {
    std::cerr << "ERROR: MapManager: given coordinates are out of bounds"
              << std::endl;
    std::cerr << "the following were provided: test_x = " << test_x
              << " , test_y = " << test_y << std::endl;
    return true;
  } else {
    return false;
  }
}
