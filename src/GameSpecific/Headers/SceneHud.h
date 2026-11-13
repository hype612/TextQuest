#ifndef SCENEHUD_H
#define SCENEHUD_H

#include "../Headers/HealthBarPresenter.h"
#include "../Headers/EnemyStatePresenter.h"
#include <memory>

struct SceneHud {
  std::unique_ptr<HealthBarPresenter> healthBar;
  std::unique_ptr<EnemyStatePresenter> enemyState;
};

#endif // SCENEHUD_H
