#ifndef NCURSESRENDERER_H
#define NCURSESRENDERER_H

#if (defined(LINUX) || defined(__linux__))

#include "IRenderer.h"
#include "Vec2i.h"
#include <ncurses.h>

class NCursesRenderer : public IRenderer {
public:
  NCursesRenderer();
  void Init() override;
  void OverwriteBuffer(char *newBuffer) override;
  void PrintBuffer() const override;
  void PrintDebugInfo(const std::vector<std::string> &nfo) override;
  vec2i screenSize() const override;
  int screenHeight() const override;
  int screenWidth() const override;

  ~NCursesRenderer();

private:
  char *_screenBuffer;
};

#endif // OS check end
#endif // NCURSESRENDERER_H
