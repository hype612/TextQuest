#ifndef NOTCURSESRENDERER_H
#define NOTCURSESRENDERER_H

#include <unordered_map>
#if (defined(LINUX) || defined(__linux__))

#include "IRenderer.h"
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
  vec2i screenSize() const override;
  int screenHeight() const override;
  int screenWidth() const override;

  // layer/plane management
  OverlayId createHudArea(Rect area) override;
  OverlayId createPopupArea(Rect area) override;
  void
  setOverlayContent(OverlayId id,
                    const std::vector<const std::string> &content) override;
  ~NotcursesRenderer();

private:
  char *_screenBuffer;
  std::shared_ptr<notcurses> _nc;
  // no explicit world plane
  // we utilize the built-in default ncplane
  // ncplane *_viewModel;
  ncplane *_debugPlane;
  ncplane *_hudPlane;
  ncplane *_popupPlane;

  enum class RenderPlane { HUD, POPUP, DEBUG };
  std::unordered_map<OverlayId, std::pair<RenderPlane, Rect>> _overdrawArea;
  OverlayId _topId = -1;
};

#endif // OS check end
#endif // NOTCURSESRENDERER_H
