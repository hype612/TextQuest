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
  int clamped = std::clamp(current, 0, _max);
  std::string label =
      " " + std::to_string(clamped) + "/" + std::to_string(_max);
  size_t barWidth = (width > label.size()) ? width - label.size() : 0;

  size_t fill = barWidth * clamped / _max;
  std::string bar(barWidth, ' ');
  std::fill_n(bar.begin(), fill, '#');

  std::string line = bar + label;
  if (line.size() < width)
    line += std::string(width - line.size(), ' ');
  return line;
}

std::vector<std::string> HealthBarPresenter::design(const Rect &area) {
  const size_t w = area.width;
  if (w < 2)
    return {};
  return {std::string(w, '='), "|" + std::string(w - 2, ' ') + "|",
          std::string(w, '=')};
}
