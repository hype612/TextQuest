#ifndef GAMEHUD_H
#define GAMEHUD_H

#include "../Headers/DamageFlashPresenter.h"
#include "../Headers/HealthBarPresenter.h"
#include "../Headers/HealthObserverFanOut.h"
#include "../Headers/EnemyStatePresenter.h"
#include "../Headers/MinimapPresenter.h"
#include "../Headers/ViewmodelPresenter.h"
#include "SceneBuilder.h"
#include "UIManager.h"
#include <memory>

class GameHud {
public:
  GameHud(UIManager &ui, vec2i screen, SceneBuilder::BuiltScene &built,
          int playerMaxHp);

  // Per-frame refresh of the parts that are not event-driven.
  void update(float dt);

  GameHud(const GameHud &) = delete;
  GameHud &operator=(const GameHud &) = delete;
  GameHud(GameHud &&) = delete;
  GameHud &operator=(GameHud &&) = delete;

private:
  std::unique_ptr<HealthBarPresenter> _healthBar;
  std::unique_ptr<EnemyStatePresenter> _enemyState;
  std::unique_ptr<MinimapPresenter> _minimap;
  std::unique_ptr<DamageFlashPresenter> _damageFlash;
  std::unique_ptr<ViewmodelPresenter> _viewmodel;
  std::unique_ptr<HealthObserverFanOut> _healthFanOut;
};

#endif // GAMEHUD_H
