#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "Rect.h"
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

class UIElement {
public:
  UIElement(Rect area, Rect writableArea, std::vector<std::string> design);

  UIElement(const UIElement &) = delete;
  UIElement &operator=(const UIElement &) = delete;
  UIElement(UIElement &&) = default;

  void updateDesign(std::vector<std::string> design);
  void updateContent(std::vector<std::string> lines);
  void setFgColor(std::optional<uint32_t> rgb);
  void setBgColor(std::optional<uint32_t> rgb);
  // Temporarily replaces both colors (nullopt = default color) for `seconds`.
  // Calling it again while active restarts the timer.
  void setColorForSec(std::optional<uint32_t> fg, std::optional<uint32_t> bg,
                      float seconds);
  // Advances the flash timer; restores the base colors when it runs out.
  void tick(float dt);

  const Rect &area() const { return _area; }
  const Rect &writableArea() const { return _writableArea; }
  const std::vector<std::string> &peekDesign() const { return _design; }
  const std::vector<std::string> &peekContent() const { return _content; }
  const std::optional<uint32_t> &fgColor() const {
    return _overrideRemaining > 0.f ? _overrideFg : _fg;
  }
  const std::optional<uint32_t> &bgColor() const {
    return _overrideRemaining > 0.f ? _overrideBg : _bg;
  }

  std::optional<std::reference_wrapper<const std::vector<std::string>>>
  takeDesign();
  std::optional<std::reference_wrapper<const std::vector<std::string>>>
  takeContent();

private:
  bool _designChanged = true;
  bool _contentChanged = true;
  Rect _area;
  Rect _writableArea;
  std::optional<uint32_t> _fg;
  std::optional<uint32_t> _bg;
  std::optional<uint32_t> _overrideFg;
  std::optional<uint32_t> _overrideBg;
  float _overrideRemaining = 0.f;

  std::vector<std::string> _design;
  std::vector<std::string> _content;
};

#endif // UIELEMENT_H
