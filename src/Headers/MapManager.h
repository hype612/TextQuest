#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "TextureMapper.h"
#include <string>
#include <unordered_map>

class MapManager {
public:
  MapManager();
  MapManager(const std::wstring &map, int map_width, int map_height);

  void uploadNewMap(const std::wstring &map, int newMapWidth, int newMapHeight);
  std::wstring GetMap();
  int mapHeight() const;
  int mapWidth() const;
  bool isWall(int test_x, int test_y) const;
  bool isMapAvailable() const;

  void uploadWallTextureFor(wchar_t mapChar, std::wstring texture);
  void rescaleWallTextureAt(int x, int y, float distance);
  std::wstring getWallTextureAt(int x, int y);
  std::wstring getWallTexColumnAt(int x, int y, int height);

private:
  std::wstring _map;
  std::unordered_map<wchar_t, TextureMapper> _wallTexMappers;
  int _mapWidth;
  int _mapHeight;
};

#endif // MAPMANAGER_H
