#include "../Headers/DamageFlashPresenter.h"
#include "../../Headers/UIElement.h"

namespace {
constexpr uint32_t kFlashRed = 0xFF3030;
constexpr float kFlashSeconds = 0.25f;
} // namespace

DamageFlashPresenter::DamageFlashPresenter(UIManager &ui,
                                           std::vector<UIElementHandle> handles,
                                           int maxHealth)
    : _ui(ui), _handles(std::move(handles)), _last(maxHealth) {}

void DamageFlashPresenter::onHealthChanged(int current) {
  bool damaged = current < _last;
  _last = current;
  if (!damaged)
    return;
  for (UIElementHandle h : _handles)
    if (UIElement *el = _ui.elementAt(h))
      el->setColorForSec(kFlashRed, std::nullopt, kFlashSeconds);
}
