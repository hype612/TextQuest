#ifndef SCENEHUD_H
#define SCENEHUD_H

#include "../Headers/HealthBarPresenter.h"
#include "../Headers/IstvanStatePresenter.h"
#include <memory>

struct SceneHud {
  std::unique_ptr<HealthBarPresenter> healthBar;
  std::unique_ptr<IstvanStatePresenter> istvanState;
};

#endif // SCENEHUD_H
