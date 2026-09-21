#ifndef HEALTHBARPRESENTER_H
#define HEALTHBARPRESENTER_H

#include "IHealthObserver.h"
#include "Rect.h"
#include "UIManager.h"
#include <string>
#include <vector>

class HealthBarPresenter : public IHealthObserver {
public:
  HealthBarPresenter(UIManager &ui, UIElementHandle handle, int maxHealth);

  void onHealthChanged(int current) override;

  static std::vector<std::string> design(const Rect &area);

private:
  std::string renderBar(int current, size_t width) const;

  UIManager &_ui;
  UIElementHandle _handle;
  int _max;
};

#endif // HEALTHBARPRESENTER_H
