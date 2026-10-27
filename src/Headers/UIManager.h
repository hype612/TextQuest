#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "IRenderer.h"
#include "UIElement.h"
#include <unordered_map>

using UIElementHandle = int;

class UIManager {
public:
  explicit UIManager(IRenderer &renderer);

  // Ticks every element's timers by `dt` seconds, then writes the dirty ones.
  void process(float dt);

  // Moves `element` into UIManager's keeping and hands back a handle for
  // it. A plain int, not an address - map storage decouples the handle
  // from any container slot.
  UIElementHandle addElement(UIElement element);

  // Tears down an element added via addElement(). Invalidates the handle -
  // further elementAt() calls with it return nullptr.
  void removeElement(UIElementHandle handle);
  void clear();
  // Resolves a handle to its element, or nullptr if the handle is
  // invalid / already removed.
  UIElement *elementAt(UIElementHandle handle);

private:
  IRenderer &_renderer;
  std::unordered_map<UIElementHandle, UIElement> _elements;
};

#endif // UIMANAGER_H
