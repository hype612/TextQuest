#include "../Headers/MapManager.h"

MapManager::MapManager() {} // so far nothing needs to be inited

MapManager::MapManager(const std::wstring &map, int mapWidth, int mapHeight)
    : _map(map), _mapWidth(mapWidth), _mapHeight(mapHeight) {}

void MapManager::uploadNewMap(const std::wstring &map, int newMapWidth,
                              int newMapHeight) {
  _map = map;
  _mapWidth = newMapWidth;
  _mapHeight = newMapHeight;
}

std::wstring MapManager::GetMap() { return _map; }

int MapManager::mapHeight() const { return _mapHeight; }
int MapManager::mapWidth() const { return _mapWidth; }

bool MapManager::isWall(int test_x, int test_y) const {
  if (_map[test_y * _mapWidth + test_x] == '#')
    return true;
  else
    return false;
}

bool MapManager::isMapAvailable() const {
  if (_map == L"") {
    return false;
  } else {
    return true;
  }
}

void MapManager::uploadWallTextureFor(const wchar_t &mapChar,
                                      std::wstring texture) {
  _wallTexMappers[mapChar] = TextureMapper(texture);
}

void MapManager::rescaleWallTextureAt(int x, int y, float distance) {
  wchar_t mapChar = _map[y * _mapWidth + x];
  _wallTexMappers[mapChar].rescaleCurrentTexture(distance);
}

std::wstring MapManager::getWallTextureAt(int x, int y) {
  wchar_t mapChar = _map[y * _mapWidth + x];
  return _wallTexMappers[mapChar].getTexture();
}

std::wstring MapManager::getWallTexColumnAt(int x, int y, int height) {
  wchar_t mapChar = _map[y * _mapWidth + x];
  return _wallTexMappers[mapChar].getNextTexColumn(height);
}
