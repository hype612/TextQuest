#ifndef NOTCURSESRENDERER_H
#define NOTCURSESRENDERER_H

#if (defined(LINUX) || defined(__linux__))

#include "IRenderer.h"
#include <memory>
#include <notcurses/notcurses.h>
#include <unordered_map>
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
  // NOTE: Everything here expcects you to pass in correct
  //       overlayIds. If you do not check your id's than
  //       may dankamongmen have mercy on your soul.
  void setOverlayContent(OverlayId id, const std::vector<std::string> &content,
                         std::optional<uint32_t> fg,
                         std::optional<uint32_t> bg) override;
  void setOverlayRegion(OverlayId id, Rect region,
                        const std::vector<std::string> &content,
                        std::optional<uint32_t> fg,
                        std::optional<uint32_t> bg) override;

  OverlayId createOverlay(Rect area) override;
  bool destroyOverlay(OverlayId id) override;
  ~NotcursesRenderer();

private:
  char *_screenBuffer;
  std::shared_ptr<notcurses> _nc;
  // does NOT contain the default plane
  ncplane *_debugPlane;
  std::unordered_map<OverlayId, ncplane *> _overlayPlanes;
  OverlayId _topId = -1;
};

#endif // OS check end
#endif // NOTCURSESRENDERER_H
