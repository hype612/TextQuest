#ifndef VIEWMODELPRESENTER_H
#define VIEWMODELPRESENTER_H

#include "IShootObserver.h"
#include "Rect.h"
#include "UIManager.h"
#include <string>
#include <vector>

// Weapon sprite at the bottom of the screen. Kicks and tints on every shot.
class ViewmodelPresenter : public IShootObserver {
public:
  ViewmodelPresenter(UIManager &ui, UIElementHandle handle);

  // Overlay rectangle for the given screen size. Kept clear of the health bar.
  static Rect area(int screenWidth, int screenHeight);
  static UIElement makeElement(const Rect &area);

  void onShoot() override;
  // Advances the recoil animation. Call once per frame.
  void update(float dt);

private:
  enum Frame { IDLE, SETTLE, RECOIL };
  void show(Frame frame);

  UIManager &_ui;
  UIElementHandle _handle;
  std::vector<std::string> _frames[3];
  float _t = -1.f; // seconds since the last shot, negative when idle
  Frame _shown = IDLE;
};

#endif // VIEWMODELPRESENTER_H
