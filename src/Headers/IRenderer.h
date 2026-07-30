#ifndef IRENDERER_H
#define IRENDERER_H

#include "Vec2i.h"
#include <string>
#include <vector>

class IRenderer {
public:
  virtual void Init() = 0;
  virtual void OverwriteBuffer(char *newBuffer) = 0;
  virtual void PrintBuffer() const = 0;

  virtual int screenHeight() const = 0;
  virtual int screenWidth() const = 0;
  virtual vec2i screenSize() const = 0;
  virtual void PrintDebugInfo(const std::vector<std::string> &nfo) = 0;
  virtual ~IRenderer() = default;

protected:
  IRenderer() {}
  int _screenHeight;
  int _screenWidth;
};

#endif // IRENDERER_H
