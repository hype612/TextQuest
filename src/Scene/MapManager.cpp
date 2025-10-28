#include "../Headers/MapManager.h"



MapManager::MapManager() {} // so far nothing needs to be inited


MapManager::MapManager(const std::wstring& map, int mapWidth, int mapHeight) 
  : _map(map), _mapHeight(mapHeight), _mapWidth(mapWidth) {}


void MapManager::uploadNewMap(const std::wstring& map, int newMapWidth, int newMapHeight) {
  _map = map;
  _mapWidth = newMapWidth;
  _mapHeight = newMapHeight;
}


std::wstring MapManager::GetMap() {
  return _map;
}

bool MapManager::isWall(int test_x, int test_y) {
  if (map[test_y * map_width + test_x] == '#')
    return true;
  else
    return false;
}

