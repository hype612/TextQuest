#include "UIElement.h"
#include "Logger.h"
#include <algorithm>

UIElement::UIElement(std::unique_ptr<IUIElementBehavior> behavior,
                     IRenderer &renderer, Rect area, Rect writableArea,
                     std::vector<std::string> design)
    : _behavior(std::move(behavior)), _renderer(renderer),
      _overlayId(renderer.createOverlay(area)), _area(area),
      _writableArea(writableArea), _design(std::move(design)) {

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

  render();
}

void UIElement::updateDesign(std::vector<std::string> design) {
  _design = std::move(design);
  render();
}

void UIElement::updateContent(std::vector<std::string> lines, vec2i offset) {
  _content = std::move(lines);
  _writeOffset = offset;
  render();
}

void UIElement::render() {
  // Start from the design as it stands, splice _content into the
  // writable rect at _writeOffset, and push the whole plane back in one
  // go - this is what keeps the design cells outside writableArea
  // untouched even though the renderer's setOverlayContent() always
  // erases+rewrites the full plane in one call.
  std::vector<std::string> merged = _design;
  merged.resize(_area.height, std::string(_area.width, ' '));
  for (auto &row : merged) {
    if (row.size() < _area.width) {
      row.resize(_area.width, ' ');
    }
  }

  for (size_t i = 0; i < _content.size(); i++) {
    int row = _writableArea.y + _writeOffset.y + static_cast<int>(i);
    if (row < _writableArea.y ||
        row >= _writableArea.y + static_cast<int>(_writableArea.height)) {
      continue; // outside writableArea - never touch a design row
    }
    if (row < 0 || row >= static_cast<int>(merged.size())) {
      continue;
    }

    const std::string &line = _content[i];
    size_t maxCols = static_cast<size_t>(_writableArea.width);
    std::string clipped = line.substr(0, std::min(line.size(), maxCols));

    int col = _writableArea.x + _writeOffset.x;
    for (size_t c = 0; c < clipped.size(); c++) {
      int cc = col + static_cast<int>(c);
      if (cc < _writableArea.x ||
          cc >= _writableArea.x + static_cast<int>(_writableArea.width)) {
        break; // outside writableArea - never touch a design column
      }
      if (cc < 0 || cc >= static_cast<int>(merged[row].size())) {
        break;
      }
      merged[row][cc] = clipped[c];
    }
  }

  _renderer.setOverlayContent(_overlayId, merged);
}
