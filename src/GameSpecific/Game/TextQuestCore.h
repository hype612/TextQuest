#ifndef TEXTQUESTCORE_H
#define TEXTQUESTCORE_H

#include "../Headers/DamageFlashPresenter.h"
#include "../Headers/HealthBarPresenter.h"
#include "../Headers/HealthObserverFanOut.h"
#include "../Headers/IstvanBehaviorController.h"
#include "../Headers/IstvanStatePresenter.h"
#include "../Headers/MinimapPresenter.h"
#include "../Headers/PlayerBehaviorController.h"
#include "../Headers/ViewmodelPresenter.h"
#include "../MapGen/cellularAutomata.hpp"
#include "Core.h"
#include "Difficulty.h"
#include "EnemyArchetype.h"
#include "IInputHandler.h"
#include "SceneManager.h"
#include "UIManager.h"
#include <functional>
#include <memory>
#include <optional>

// The game itself. Owns everything scene-scoped (HUD, presenters) and holds
// references to the engine services, which live as long as the GameEngine.
// The engine callbacks capture `this`, so the object must stay where it is.
class TextQuestCore {
public:
  explicit TextQuestCore(GameEngine &ge);
  void run();

  void registerEnemyArchetype(std::string name, EnemyArchetype type);
  // Builds the controller of a new enemy from its difficulty-scaled archetype.
  using ControllerFactory = std::function<std::unique_ptr<IBehaviorController>(
      const EnemyArchetype &)>;
  void spawnEnemy(const std::string &type_key, SceneManager &scene, vec2f pos,
                  const ControllerFactory &make_ctrl);

  TextQuestCore(const TextQuestCore &) = delete;
  TextQuestCore &operator=(const TextQuestCore &) = delete;
  TextQuestCore(TextQuestCore &&) = delete;
  TextQuestCore &operator=(TextQuestCore &&) = delete;

private:
  // The controllers are owned by their entities inside the scene. The raw
  // pointers are handed out so the HUD can attach presenters to them.
  struct BuiltScene {
    std::shared_ptr<SceneManager> scene;
    PlayerBehaviorController *playerCtrl;
    IstvanBehaviorController *istvanCtrl;
  };

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

  BuiltScene buildScene();
  SceneHud buildHud(BuiltScene &built);
  void startScene();
  void onSceneOver();
  void onPostSceneOver();
  void loadResources(const std::string &filePath,
                     std::unordered_map<std::string, std::string> &outTextures);

  std::vector<std::string> loadTextureVec(const std::string &dir);
  std::string serializeThemedMap(const std::vector<std::vector<Tile>> &map,
                                 const std::vector<Rect> &leafRooms,
                                 const std::vector<char> &wallThemes) const;
  static constexpr int playerMaxHp = 100;
  static constexpr const char *seedFile = "last_seed.txt";

  // Seed of the current map. Kept so that retrying replays the same map.
  unsigned int _seed = 0;

  std::unordered_map<std::string, EnemyArchetype> _enemyArchetypes;
  Difficulty _chosenDifficulty = NORMAL;
  GameEngine &_ge;
  UIManager &_ui;
  IInputHandler &_input;
  SceneHud _hud;
};

#endif // TEXTQUESTCORE_H
