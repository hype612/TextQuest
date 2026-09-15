#include "UIManager.h"
#include <utility>

UIManager::UIManager(IRenderer &renderer) : _renderer(renderer) {}

UIElementHandle UIManager::addElement(UIElement element) {
  UIElementHandle handle = _nextId++;
  _elements.emplace(handle, std::move(element));
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

void UIManager::process(float delta) {
  for (auto &[id, element] : _elements) {
    element.Update(delta);
  }
}
