#ifndef MAPMANAGER_H
#define MAPMANAGER_H


#include <string>
#include <unordered_map>
#include "TextureMapper.h"


class MapManager
{
public:
  MapManager();
  MapManager(const std::wstring& map, int map_width, int map_height);

  void uploadNewMap(const std::wstring& map, int newMapWidth, int newMapHeight);
  std::wstring GetMap(const std::wstring& map);
  bool isWall();
 
  void uploadWallTextureFor(wchar_t mapChar, std::wstring texture);
  void rescaleWallTextureAt(int x, int y, float distance);
  std::wstring getWallTextureAt(int x, int y);
  std::wstring getWallTexColumnAt(int x, int y);
private:
  std::wstring _map;
  std::unordered_map<wchar_t, TextureMapper>  _wallTexMappers;
  int _mapWidth;
  int _mapHeight;
}

#endif // MAPMANAGER_H 
