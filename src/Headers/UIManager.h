#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "IRenderer.h"
#include "UIElement.h"
#include <unordered_map>

using UIElementHandle = int;

class UIManager {
public:
  explicit UIManager(IRenderer &renderer);

  void process(float delta);

  // Moves `element` into UIManager's keeping and hands back a handle for
  // it. A plain int, not an address - map storage decouples the handle
  // from any container slot, so the programmer can stash/iterate handles
  // on their own side without caring how UIManager stores things
  // internally.
  //
  //   UIElementHandle hp = uiManager.addElement(
  //       UIElement(std::make_unique<HealthBarBehavior>(), renderer, area,
  //                 writableArea, design));
  UIElementHandle addElement(UIElement element);

  // Tears down an element added via addElement(). Invalidates the handle -
  // further elementAt() calls with it return nullptr.
  // NOTE: needs IRenderer to grow a matching destroyOverlay(OverlayId) -
  // not there yet.
  void removeElement(UIElementHandle handle);

  // Resolves a handle to its element, or nullptr if the handle is
  // invalid / already removed.
  UIElement *elementAt(UIElementHandle handle);

private:
  IRenderer &_renderer;
  std::unordered_map<UIElementHandle, UIElement> _elements;
  UIElementHandle _nextId = 0;
};

#endif // UIMANAGER_H
