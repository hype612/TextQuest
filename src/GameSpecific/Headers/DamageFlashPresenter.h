#ifndef DAMAGEFLASHPRESENTER_H
#define DAMAGEFLASHPRESENTER_H

#include "IHealthObserver.h"
#include "UIManager.h"
#include <vector>

class DamageFlashPresenter : public IHealthObserver {
public:
  DamageFlashPresenter(UIManager &ui, std::vector<UIElementHandle> handles,
                       int maxHealth);

  void onHealthChanged(int current) override;

private:
  UIManager &_ui;
  std::vector<UIElementHandle> _handles;
  int _last;
};

#endif // DAMAGEFLASHPRESENTER_H
