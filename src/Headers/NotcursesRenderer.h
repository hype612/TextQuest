#ifndef NOTCURSESRENDERER_H
#define NOTCURSESRENDERER_H

#if (defined(LINUX) || defined(__linux__))

#include "IRenderer.h"
#include "player.h"
#include <memory>
#include <notcurses/notcurses.h>
#include <vector>

class NotcursesRenderer : public IRenderer {
public:
  NotcursesRenderer(std::shared_ptr<notcurses> nc);
  void Init() override;
  void OverwriteBuffer(char *newBuffer) override;
  void PrintBuffer() const override;
  void PrintDebugInfo(const std::vector<std::string> &nfo) override;
  std::tuple<int, int> GetScreenSize() const override;
  // void SetScreenSize(int x, int y) override;
  ~NotcursesRenderer();

private:
  char *_screenBuffer;
  std::shared_ptr<notcurses> _nc;
  ncplane *_debugPln;
};

#endif // OS check end
#endif // NOTCURSESRENDERER_H
