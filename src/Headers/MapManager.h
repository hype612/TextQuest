#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "TextureMapper.h"
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

class MapManager {
public:
  MapManager();
  MapManager(const std::string &map, int map_width, int map_height);

  // =======================
  // Rendering setters
  // =======================
  void uploadWallTextureFor(const char &mapChar, std::string texture);
  void uploadWallTextureVecFor(const char &mapChar,
                               std::vector<std::string> textureV);

  // =======================
  // Rendering getters
  // =======================

  // Both only used for debugging purposes
  // generally you do not really want to do anything
  // with the whole texture at this point in the pipeline
  std::optional<std::reference_wrapper<const std::string>>
  getWallTextureAt(int x, int y) const;

  std::optional<std::reference_wrapper<const std::string>>
  getWallTexForMapChar(const char &mapChar) const;
  // Main rendering functional
  // ShadingIdx version for distance based shading
  std::string getWallTexColumnAt(int x, int y, int height, float hitpoint,
                                 int wallTop) const;
  std::string getWallTexColumnAt(int x, int y, int height, float hitpoint,
                                 int wallTop, int shadingIdx) const;

  // =======================
  // Map setters
  // =======================

  void uploadNewMap(const std::string &map, int newMapWidth, int newMapHeight);

  // =======================
  // Map getters
  // =======================

  const std::string &GetMap() const;
  int mapHeight() const;
  int mapWidth() const;
  bool isWall(int test_x, int test_y) const;
  bool isMapAvailable() const;
  bool isOutOfBounds(int test_x, int test_y) const;

private:
  std::string _map;
  std::unordered_map<char, TextureMapper> _wallTexMappers;
  int _mapWidth;
  int _mapHeight;
};

#endif // MAPMANAGER_H
