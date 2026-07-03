#ifndef NCURSESRENDERER_H
#define NCURSESRENDERER_H

#if (defined(LINUX) || defined(__linux__))

#include "./IRenderer.h"
#include "./player.h"
#include <iostream>
#include <ncurses.h>

class NCursesRenderer : public IRenderer {
public:
  NCursesRenderer();
  void Init() override;
  void OverwriteBuffer(char *newBuffer) override;
  void PrintBuffer() override;
  void PrintDebugInfo(const Player &player, float delta) override;
  std::tuple<int, int> GetScreenSize() override;
  void SetScreenSize(int x, int y) override;

  ~NCursesRenderer();

private:
  char *_screenBuffer;
};

#endif // OS check end
#endif // NCURSESRENDERER_H
