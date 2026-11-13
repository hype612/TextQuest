#ifndef TQ_TEST_FAKERENDERER_H
#define TQ_TEST_FAKERENDERER_H

#include "IRenderer.h"
#include <optional>
#include <string>
#include <vector>

// IRenderer that draws nothing and records every call made to it.
class FakeRenderer : public IRenderer {
public:
  struct CreateCall {
    Rect area;
    OverlayId result;
  };
  struct SetCall {
    OverlayId id;
    std::vector<std::string> content;
    std::optional<uint32_t> fg, bg;
    bool transparentSpaces;
  };
  struct RegionCall {
    OverlayId id;
    Rect region;
    std::vector<std::string> content;
    std::optional<uint32_t> fg, bg;
    bool transparentSpaces;
  };
  struct VisibleCall {
    OverlayId id;
    bool visible;
  };

  explicit FakeRenderer(int width = 80, int height = 24) {
    _screenWidth = width;
    _screenHeight = height;
  }

  void Init() override {}
  void OverwriteBuffer(char *newBuffer) override {
    lastBuffer.assign(newBuffer, newBuffer + _screenWidth * _screenHeight);
    ++overwriteCount;
  }
  void PrintBuffer() const override { ++printCount; }
  int screenHeight() const override { return _screenHeight; }
  int screenWidth() const override { return _screenWidth; }
  vec2i screenSize() const override { return {_screenWidth, _screenHeight}; }

  OverlayId createOverlay(Rect area) override {
    OverlayId id = -1;
    if (rejectNext)
      rejectNext = false;
    else
      id = _nextId++;
    creates.push_back({area, id});
    return id;
  }
  void setOverlay(OverlayId id, const std::vector<std::string> &content,
                  std::optional<uint32_t> fg, std::optional<uint32_t> bg,
                  bool transparentSpaces) override {
    sets.push_back({id, content, fg, bg, transparentSpaces});
  }
  void setOverlayRegion(OverlayId id, Rect region,
                        const std::vector<std::string> &content,
                        std::optional<uint32_t> fg, std::optional<uint32_t> bg,
                        bool transparentSpaces) override {
    regions.push_back({id, region, content, fg, bg, transparentSpaces});
  }
  bool destroyOverlay(OverlayId id) override {
    destroyed.push_back(id);
    return true;
  }
  bool setOverlayVisible(OverlayId id, bool visible) override {
    visibility.push_back({id, visible});
    return true;
  }

  // Recorded calls
  std::vector<CreateCall> creates;
  std::vector<SetCall> sets;
  std::vector<RegionCall> regions;
  std::vector<OverlayId> destroyed;
  std::vector<VisibleCall> visibility;
  std::vector<char> lastBuffer;
  mutable int printCount = 0;
  int overwriteCount = 0;


  bool rejectNext = false;


private:
  OverlayId _nextId = 0;
};

#endif // TQ_TEST_FAKERENDERER_H
