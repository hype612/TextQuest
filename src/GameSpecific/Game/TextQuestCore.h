#ifndef TEXTQUESTCORE_H
#define TEXTQUESTCORE_H

#include "../Headers/DamageFlashPresenter.h"
#include "../Headers/HealthBarPresenter.h"
#include "../Headers/HealthObserverFanOut.h"
#include "../Headers/IstvanStatePresenter.h"
#include "../Headers/MinimapPresenter.h"
#include "../Headers/ViewmodelPresenter.h"
#include "Core.h"
#include "Difficulty.h"
#include "IInputHandler.h"
#include "SceneBuilder.h"
#include "UIManager.h"
#include <memory>
#include <optional>

// The game itself. Owns everything scene-scoped (HUD, presenters) and holds
// references to the engine services, which live as long as the GameEngine.
// The engine callbacks capture `this`, so the object must stay where it is.
class TextQuestCore {
public:
  explicit TextQuestCore(GameEngine &ge);
  void run();

  TextQuestCore(const TextQuestCore &) = delete;
  TextQuestCore &operator=(const TextQuestCore &) = delete;
  TextQuestCore(TextQuestCore &&) = delete;
  TextQuestCore &operator=(TextQuestCore &&) = delete;

private:
  using BuiltScene = SceneBuilder::BuiltScene;

  // Per-scene HUD. The presenters observe the scene's controllers, so they
  // are created with the scene and replaced when it is reloaded.
  struct SceneHud {
    std::unique_ptr<HealthBarPresenter> healthBar;
    std::unique_ptr<IstvanStatePresenter> istvanState;
    std::unique_ptr<MinimapPresenter> minimap;
    std::unique_ptr<DamageFlashPresenter> damageFlash;
    std::unique_ptr<ViewmodelPresenter> viewmodel;
    std::unique_ptr<HealthObserverFanOut> healthFanOut;
  };

  enum class MenuChoice { NEW_GAME, LAST_SEED, EXIT };

  // Shows the start menu and blocks until an option is picked.
  MenuChoice showMenu(std::optional<unsigned int> savedSeed);
  // Shows the difficulty picker and blocks until an option is picked. Returns
  bool showDifficultyMenu();
  std::optional<unsigned int> loadLastSeed() const;
  void saveSeed(unsigned int seed) const;

  SceneHud buildHud(BuiltScene &built);
  void startScene();
  void onSceneOver();
  void onPostSceneOver();

  static constexpr const char *seedFile = "last_seed.txt";

  // Seed of the current map. Kept so that retrying replays the same map.
  unsigned int _seed = 0;

  Difficulty _chosenDifficulty = NORMAL;
  GameEngine &_ge;
  UIManager &_ui;
  IInputHandler &_input;
  SceneBuilder _builder;
  SceneHud _hud;
};

#endif // TEXTQUESTCORE_H
