#ifndef NCURSESRENDERER_H
#define NCURSESRENDERER_H

#if (defined(LINUX) || defined(__linux__))

#include "./IRenderer.h"
#include <iostream>
#include <ncurses.h>

class NCursesRenderer : public IRenderer {
public:
  NCursesRenderer();
  void Init() override;
  void OverwriteBuffer(wchar_t *newBuffer) override;
  void PrintBuffer() override;

  std::tuple<int, int> GetScreenSize() override;
  void SetScreenSize(int x, int y) override;

  ~NCursesRenderer();
};

#endif // OS check end
#endif // NCURSESRENDERER_H
