#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "IRenderer.h"
#include "IUIElementBehavior.h"
#include "Rect.h"
#include "Vec2i.h"
#include <memory>
#include <string>
#include <vector>

// The one concrete UI element type - no subclassing. Has the renderer
// carve out its own overlay and writes `design` into it at construction,
// then exposes the writable remainder - `writableArea`, a sub-rect of
// `area` in the plane's own local coordinates - so callers know exactly
// how much raw content they're allowed to hand back.
//
// design and content are kept as genuinely separate state, each with its
// own entry point (updateDesign / updateContent) - both funnel into the
// same private render() so the plane is always design-with-content-spliced-
// on-top, but neither call touches the other's data.
//
// Per-widget behavior (a health bar ticking down, a dialogue box
// scrolling, ...) is composed in via IUIElementBehavior rather than
// expressed as a UIElement subclass - see IUIElementBehavior.h.
class UIElement {
public:
  UIElement(std::unique_ptr<IUIElementBehavior> behavior, IRenderer &renderer,
            Rect area, Rect writableArea, std::vector<std::string> design);

  // Move-only: it owns a live OverlayId, copying would leave two UIElements
  // pointing at the same overlay.
  UIElement(const UIElement &) = delete;
  UIElement &operator=(const UIElement &) = delete;
  UIElement(UIElement &&) = default;

  void Update(float delta) { _behavior->Tick(*this, delta); }

  // Replaces the design outright and re-renders (current content is kept
  // and re-spliced on top).
  void updateDesign(std::vector<std::string> design);

  // Writes `lines` into writableArea only, at `offset` cells into that
  // rect, and re-renders. Never touches a cell outside writableArea, i.e.
  // never touches the design.
  void updateContent(std::vector<std::string> lines, vec2i offset = {0, 0});

  const Rect &getArea() const { return _area; }
  const Rect &getWritableArea() const { return _writableArea; }
  const std::vector<std::string> &getDesign() const { return _design; }
  const std::vector<std::string> &getContent() const { return _content; }

private:
  // Splices _content into the writable rect of _design and pushes the
  // whole plane back in one go - this is what keeps the design cells
  // outside writableArea untouched even though the renderer's
  // setOverlayContent() always erases+rewrites the full plane in one call.
  void render();

  std::unique_ptr<IUIElementBehavior> _behavior;
  IRenderer &_renderer;
  OverlayId _overlayId;

  Rect _area;
  Rect _writableArea;
  vec2i _writeOffset{0, 0};

  std::vector<std::string> _design;
  std::vector<std::string> _content;
};

#endif // UIELEMENT_H
