#include "../Headers/MinimapPresenter.h"
#include "../../Headers/UIElement.h"
#include <cmath>

MinimapPresenter::MinimapPresenter(UIManager &ui, UIElementHandle handle,
                                   const SceneManager &scene,
                                   const Transform &player)
    : _ui(ui), _handle(handle), _scene(scene), _player(player) {
  update();
}

void MinimapPresenter::update() {
  UIElement *el = _ui.elementAt(_handle);
  if (!el)
    return; // element was removed or never created

  const int w = static_cast<int>(el->writableArea().width);
  const int h = static_cast<int>(el->writableArea().height);
  const int px = static_cast<int>(std::floor(_player.position.x));
  const int py = static_cast<int>(std::floor(_player.position.y));
  const int originX = px - w / 2;

  // The engine world is y-up (see the camera plane in RayCastingProcess), so
  // higher map y is drawn higher on screen: rows run top-down, y bottom-up.
  std::vector<std::string> lines(h, std::string(w, ' '));
  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col++) {
      int mx = originX + col;
      int my = py + h / 2 - row;
      if (!_scene.isOutOfBounds(mx, my) && _scene.isWall(mx, my))
        lines[row][col] = '#';
    }
  }

  for (const Entity &e : _scene.entities()) {
    if (e.isPlayer() || e.health() <= 0)
      continue;
    int col = static_cast<int>(std::floor(e.transform().position.x)) - originX;
    int row = py + h / 2 - static_cast<int>(std::floor(e.transform().position.y));
    if (col >= 0 && col < w && row >= 0 && row < h)
      lines[row][col] = 'e';
  }
  lines[h / 2][w / 2] = playerArrow();

  if (lines == _last)
    return;
  _last = lines;
  el->updateContent(std::move(lines));
}

// Facing is (sin a, cos a) in map space; +y is drawn upwards (see update()).
char MinimapPresenter::playerArrow() const {
  const float dx = std::sin(_player.angle);
  const float dy = std::cos(_player.angle);
  if (std::abs(dx) > std::abs(dy))
    return dx > 0.f ? '>' : '<';
  return dy > 0.f ? '^' : 'v';
}

std::vector<std::string> MinimapPresenter::design(const Rect &area) {
  const size_t w = area.width;
  const size_t h = area.height;
  if (w < 2 || h < 2)
    return {};
  std::vector<std::string> out;
  out.emplace_back(w, '=');
  for (size_t i = 0; i + 2 < h; i++)
    out.push_back("|" + std::string(w - 2, ' ') + "|");
  out.emplace_back(w, '=');
  return out;
}
