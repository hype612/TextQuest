#ifndef MAPMANAGER_H
#define MAPMANAGER_H


#include <string>


class MapManager 
{
public:
  MapManager();
  MapManager(const std::wstring& map, int map_width, int map_height);

  void uploadNewMap(const std::wstring& map, int newMapWidth, int newMapHeight);
  std::wstring GetMap(const std::wstring& map);
  bool isWall();

private:
  std::wstring _map;
  int _mapWidth;
  int _mapHeight;
}



#endif // MAPMANAGER_H 
