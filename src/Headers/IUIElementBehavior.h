#ifndef IUIELEMENTBEHAVIOR_H
#define IUIELEMENTBEHAVIOR_H

class UIElement;

// Mirrors IBehaviorController for entities: UIElement is one concrete
// class, and per-widget behavior (a health bar ticking down, a dialogue
// box scrolling, ...) is composed in rather than expressed as a UIElement
// subclass. Keeps UIManager free of templates - it only ever stores and
// hands back UIElement itself.
class IUIElementBehavior {
public:
  virtual ~IUIElementBehavior() = default;
  virtual void Tick(UIElement &self, float delta) = 0;
};

#endif // IUIELEMENTBEHAVIOR_H
