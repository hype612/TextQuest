#include "UIElement.h"
#include "Logger.h"
#include <optional>

UIElement::UIElement(Rect area, Rect writableArea,
                     std::vector<std::string> design,
                     bool transparentSpaces)
    : _area(area), _writableArea(writableArea),
      _design(std::move(design)),
      _transparentSpaces(transparentSpaces) {

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

void UIElement::setFgColor(std::optional<uint32_t> rgb) {
  if (_fg == rgb)
    return;
  _fg = rgb;
  _designChanged = true;
}

void UIElement::setBgColor(std::optional<uint32_t> rgb) {
  if (_bg == rgb)
    return;
  _bg = rgb;
  _designChanged = true;
}

void UIElement::setColorForSec(std::optional<uint32_t> fg,
                               std::optional<uint32_t> bg, float seconds) {
  if (seconds <= 0.f)
    return;
  if (fgColor() != fg || bgColor() != bg)
    _designChanged = true;
  _overrideFg = fg;
  _overrideBg = bg;
  _colorOverrideRemaining = seconds;
}

void UIElement::setContentForSec(const std::vector<std::string> &override,
                                 float seconds) {
  if (seconds <= 0.f)
    return;
  if (peekContent() != override)
    _contentChanged = true;
  _overrideContent = override;
  _contentOverrideRemaining = seconds;
}

void UIElement::tick(float dt) {
  if (_colorOverrideRemaining > 0.f) {
    _colorOverrideRemaining -= dt;
    if (_colorOverrideRemaining <= 0.f) {
      _designChanged = true; // back to the base colors
    }
  }  

  if (_contentOverrideRemaining > 0.f) {
    _contentOverrideRemaining -= dt;
    if (_contentOverrideRemaining <= 0.f) {
      _contentChanged = true; // back to the base content
    }
  }  
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
  return _contentOverrideRemaining > 0.f ? _overrideContent : _content;
}
