#ifndef ENEMYSTATEPRESENTER_H
#define ENEMYSTATEPRESENTER_H

#include "IEnemyStateObserver.h"
#include "Rect.h"
#include "UIManager.h"
#include <string>
#include <vector>

class EnemyStatePresenter : public IEnemyStateObserver {
public:
  EnemyStatePresenter(UIManager &ui, UIElementHandle handle);

  void onStateChanged(const std::string &state) override;

  static std::vector<std::string> design(const Rect &area);

private:
  UIManager &_ui;
  UIElementHandle _handle;
};

#endif // ENEMYSTATEPRESENTER_H
