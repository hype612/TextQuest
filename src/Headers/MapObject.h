#ifndef MAPOBJECT_H
#define MAPOBJECT_H

#include <tuple>
#include <string>

#include "../Headers/TextureMapper.h"

class MapObject {
public:
  int X();
  int Y();
  std::tuple<int, int> Pos();
  std::wstring Texture();
protected:
  int _x;
  int _y;
  std::tuple<int, int> _pos;
  std::wstring _texture;
  TextureMapper _tMapper;
};


#endif // MAPOBJECT_H
