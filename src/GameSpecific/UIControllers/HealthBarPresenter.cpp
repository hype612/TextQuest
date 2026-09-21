#include "../Headers/HealthBarPresenter.h"
#include "../../Headers/UIElement.h"
#include <algorithm>

HealthBarPresenter::HealthBarPresenter(UIManager &ui, UIElementHandle handle,
                                       int maxHealth)
    : _ui(ui), _handle(handle), _max(maxHealth) {
  onHealthChanged(maxHealth);
}

void HealthBarPresenter::onHealthChanged(int current) {
  if (_max <= 0)
    return;
  UIElement *el = _ui.elementAt(_handle);
  if (!el)
    return; // element was removed or never created
  el->updateContent({renderBar(current, el->writableArea().width)});
}

std::string HealthBarPresenter::renderBar(int current, size_t width) const {
  size_t fill = width * std::clamp(current, 0, _max) / _max;
  std::string bar(width, ' ');
  std::fill_n(bar.begin(), fill, '#');
  return bar;
}

std::vector<std::string> HealthBarPresenter::design(const Rect &area) {
  const size_t w = area.width;
  if (w < 2)
    return {};
  return {std::string(w, '='), "|" + std::string(w - 2, ' ') + "|",
          std::string(w, '=')};
}
