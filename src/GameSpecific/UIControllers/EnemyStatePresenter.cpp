#include "../Headers/EnemyStatePresenter.h"
#include "../../Headers/UIElement.h"

EnemyStatePresenter::EnemyStatePresenter(UIManager &ui,
                                           UIElementHandle handle)
    : _ui(ui), _handle(handle) {}

void EnemyStatePresenter::onStateChanged(const std::string &state) {
  UIElement *el = _ui.elementAt(_handle);
  if (!el)
    return; // element was removed or never created
  // Enemy never returns to IDLE once he has seen the player, so the first
  // non-IDLE state is the first sighting: reveal the (hidden) element.
  if (state != "IDLE")
    _ui.setVisible(_handle, true);
  std::string line = "Enemy: " + state;
  size_t width = el->writableArea().width;
  if (line.size() < width)
    line += std::string(width - line.size(), ' ');
  else if (line.size() > width)
    line.resize(width);
  el->updateContent({line});
}

std::vector<std::string> EnemyStatePresenter::design(const Rect &area) {
  const size_t w = area.width;
  if (w < 2)
    return {};
  return {std::string(w, '='), "|" + std::string(w - 2, ' ') + "|",
          std::string(w, '=')};
}
