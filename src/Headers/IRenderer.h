#ifndef IRENDERER_H
#define IRENDERER_H

#include "Rect.h"
#include "Vec2i.h"
#include <string>
#include <vector>

using OverlayId = int;

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

  // TODO: make this more generic
  // Rendered should not care about what you are rendering
  // virtual OverlayId createHudArea(Rect area) = 0;
  // virtual OverlayId createPopupArea(Rect area) = 0;
  // virtual void setOverlayContent(OverlayId id,
  //                              const std::vector<std::string> &content) = 0;

  virtual OverlayId createOverlay(Rect area) = 0;

protected:
  IRenderer() {}
  int _screenHeight;
  int _screenWidth;
};

#endif // IRENDERER_H
