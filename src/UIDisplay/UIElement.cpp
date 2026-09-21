#include "UIElement.h"
#include "Logger.h"
#include <optional>

UIElement::UIElement(Rect area, Rect writableArea,
                     std::vector<std::string> design)
    : _area(area), _writableArea(writableArea), _design(std::move(design)) {

  bool fitsInArea =
      writableArea.x >= 0 && writableArea.y >= 0 &&
      static_cast<unsigned int>(writableArea.x) + writableArea.width <=
          area.width &&
      static_cast<unsigned int>(writableArea.y) + writableArea.height <=
          area.height;
  if (!fitsInArea) {
    Logger::GetInstance()->log(
        "UIElement: writableArea does not fit inside area, clamping will "
        "occur on write.",
        LogType::RENDER, LogLevel::WARNING);
  }
}

void UIElement::updateDesign(std::vector<std::string> design) {
  _design = std::move(design);
  _designChanged = true;
}

void UIElement::updateContent(std::vector<std::string> lines) {
  _content = std::move(lines);
  _contentChanged = true;
}

std::optional<std::reference_wrapper<const std::vector<std::string>>>
UIElement::takeDesign() {
  if (!_designChanged) {
    return std::nullopt;
  }
  _designChanged = false;
  return _design;
}
std::optional<std::reference_wrapper<const std::vector<std::string>>>
UIElement::takeContent() {
  if (!_contentChanged) {
    return std::nullopt;
  }
  _contentChanged = false;
  return _content;
}
