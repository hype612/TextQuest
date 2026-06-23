#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "TextureMapper.h"
#include <string>
#include <unordered_map>

class MapManager {
public:
  MapManager();
  MapManager(const std::string &map, int map_width, int map_height);

  void uploadNewMap(const std::string &map, int newMapWidth, int newMapHeight);
  const std::string &GetMap() const;
  int mapHeight() const;
  int mapWidth() const;
  bool isWall(int test_x, int test_y) const;
  bool isMapAvailable() const;
  const std::string &getWallTexForMapChar(const char &mapChar);

  void uploadWallTextureFor(const char &mapChar, std::string texture);
  void uploadWallTextureVecFor(const char &mapChar,
                               std::vector<std::string> textureV);
  std::string getWallTextureAt(int x, int y);
  std::string getWallTexColumnAt(int x, int y, int height, float hitpoint);
  std::string getWallTexColumnAt(int x, int y, int height, float hitpoint,
                                 int shadingIdx);
  bool isOutOfBounds(int test_x, int test_y) const;

private:
  std::string _map;
  std::unordered_map<char, TextureMapper> _wallTexMappers;
  int _mapWidth;
  int _mapHeight;
};

#endif // MAPMANAGER_H
