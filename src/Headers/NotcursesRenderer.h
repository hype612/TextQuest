#ifndef NOTCURSESRENDERER_H
#define NOTCURSESRENDERER_H

#if (defined(LINUX) || defined(__linux__))

#include "IRenderer.h"
#include "player.h"
#include <memory>
#include <notcurses/notcurses.h>

class NotcursesRenderer : public IRenderer {
public:
  NotcursesRenderer(std::shared_ptr<notcurses> nc);
  void Init() override;
  void OverwriteBuffer(char *newBuffer) override;
  void PrintBuffer() override;
  void PrintDebugInfo(const Player &player, float delta) override;
  std::tuple<int, int> GetScreenSize() override;
  // void SetScreenSize(int x, int y) override;
  ~NotcursesRenderer();

private:
  char *_screenBuffer;
  std::shared_ptr<notcurses> _nc;
  ncplane *_debugPln;
};

#endif // OS check end
#endif // NOTCURSESRENDERER_H
