#ifndef TEXTUREREQUEST_H
#define TEXTUREREQUEST_H

#include "Tile.h"

struct TextureRequest {
public:
  int mapX;
  int mapY;
  Tile tileType;
  int height;
  int wallTop;
  float hitPoint;
  float distance;
};

#endif // TEXTUREREQUEST_H
