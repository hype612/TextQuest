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

  const Rect &area() const { return _area; }
  const Rect &writableArea() const { return _writableArea; }
  const std::vector<std::string> &peekDesign() const { return _design; }
  const std::vector<std::string> &peekContent() const { return _content; }
  const std::optional<uint32_t> &fgColor() const { return _fg; };
  const std::optional<uint32_t> &bgColor() const { return _bg; };

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

  std::vector<std::string> _design;
  std::vector<std::string> _content;
};

#endif // UIELEMENT_H
