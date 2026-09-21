#include "UIManager.h"
#include "Logger.h"
#include <utility>

UIManager::UIManager(IRenderer &renderer) : _renderer(renderer) {}

UIElementHandle UIManager::addElement(UIElement element) {
  UIElementHandle handle = _renderer.createOverlay(element.area());
  if (handle == -1) {
    Logger::GetInstance()->log("addElement: UIElement could not be created.",
                               LogType::RENDER, LogLevel::ERROR);
  } else {
    _elements.emplace(handle, std::move(element));
  }
  return handle;
}

void UIManager::removeElement(UIElementHandle handle) {
  // NOTE: this only drops UIManager's ownership of the element - the
  // underlying ncplane isn't destroyed yet, IRenderer has no
  // destroyOverlay(OverlayId) counterpart to createOverlay() yet.
  _elements.erase(handle);
}

UIElement *UIManager::elementAt(UIElementHandle handle) {
  auto it = _elements.find(handle);
  return it == _elements.end() ? nullptr : &it->second;
}

void UIManager::process() {
  if (_elements.empty()) {
    return;
  }
  for (auto &[id, element] : _elements) {
    if (auto design = element.takeDesign()) {
      _renderer.setOverlayContent(id, design->get());
      if (auto content = element.takeContent()) {
        _renderer.setOverlayRegion(id, element.writableArea(), content->get());
      } else {
        _renderer.setOverlayRegion(id, element.writableArea(),
                                   element.peekContent());
      }
    } else if (auto d = element.takeContent()) {
      _renderer.setOverlayRegion(id, element.writableArea(), d->get());
    }
  }
}
