#include "../Headers/ViewmodelPresenter.h"
#include "../../Headers/UIElement.h"
#include <algorithm>

namespace {
constexpr unsigned int width = 24;
constexpr unsigned int height = 10;
constexpr uint32_t idle_color = 0x2A9D4B;
constexpr uint32_t muzzle_color = 0x50FF50;
constexpr float recoil_seconds = 0.05f;
constexpr float settle_seconds = 0.10f; // end of the animation
constexpr float tint_seconds = 0.10f;
constexpr int min_x = 52; // right of the health bar

const std::vector<std::string> art = {
    "        .-########-.",    "        |||::::::|||",
    "        |||::::::|||",    "        '-#|@=|@|#-'",
    "       /###|&=|x|###\\",  "      /####|====|####\\",
    "     |################|", "          |######|",
};
const std::string muzzle_flash = "           \\\\||//";

// Bottom-aligned art, pushed `drop` rows down (clipped), optionally with a
// muzzle flash above the barrel.
std::vector<std::string> buildFrame(int drop, bool flash) {
  std::vector<std::string> rows(height, std::string(width, ' '));
  int top = static_cast<int>(height) - static_cast<int>(art.size()) + drop;
  for (size_t i = 0; i < art.size(); i++) {
    int r = top + static_cast<int>(i);
    if (r < 0 || r >= static_cast<int>(height))
      continue;
    rows[r].replace(0, art[i].size(), art[i]);
  }
  if (flash && top - 1 >= 0)
    rows[top - 1].replace(0, muzzle_flash.size(), muzzle_flash);
  return rows;
}
} // namespace

ViewmodelPresenter::ViewmodelPresenter(UIManager &ui, UIElementHandle handle)
    : _ui(ui), _handle(handle) {
  _frames[IDLE] = buildFrame(0, false);
  _frames[SETTLE] = buildFrame(1, false);
  _frames[RECOIL] = buildFrame(2, true);
  show(IDLE);
}

Rect ViewmodelPresenter::area(int screenWidth, int screenHeight) {
  int x = std::max(min_x, (screenWidth - static_cast<int>(width)) / 2);
  return Rect{x, screenHeight - static_cast<int>(height) - 1, width, height};
}

UIElement ViewmodelPresenter::makeElement(const Rect &area) {
  UIElement el(area, Rect{0, 0, width, height},
               std::vector<std::string>(height, std::string(width, ' ')),
               /*transparentSpaces=*/true);
  el.setFgColor(idle_color);
  return el;
}

void ViewmodelPresenter::show(Frame frame) {
  _shown = frame;
  if (UIElement *el = _ui.elementAt(_handle))
    el->updateContent(_frames[frame]);
}

void ViewmodelPresenter::onShoot() {
  _t = 0.f;
  show(RECOIL);
  if (UIElement *el = _ui.elementAt(_handle))
    el->setColorForSec(muzzle_color, std::nullopt, tint_seconds);
}

void ViewmodelPresenter::update(float dt) {
  if (_t < 0.f)
    return;
  _t += dt;
  Frame target = _t < recoil_seconds   ? RECOIL
                 : _t < settle_seconds ? SETTLE
                                       : IDLE;
  if (target == IDLE)
    _t = -1.f;
  if (target != _shown)
    show(target);
}
