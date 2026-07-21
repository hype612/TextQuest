#ifndef TEXTUREREQUEST_H
#define TEXTUREREQUEST_H

struct TextureRequest {
public:
  int mapX;
  int mapY;
  int height;
  int visibleTop;
  int visibleBot;
  float hitPoint;
  float distance;
};

#endif // TEXTUREREQUEST_H
