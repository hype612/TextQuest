#ifndef TEXTQUESTCORE_H
#define TEXTQUESTCORE_H

#include "../Headers/HealthBarPresenter.h"
#include "../Headers/IstvanBehaviorController.h"
#include "../Headers/IstvanStatePresenter.h"
#include "../Headers/PlayerBehaviorController.h"
#include "Core.h"
#include "IInputHandler.h"
#include "SceneManager.h"
#include "UIManager.h"
#include <memory>

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
  };

  BuiltScene buildScene();
  SceneHud buildHud(BuiltScene &built);
  void startScene();
  void onSceneOver();
  void onPostSceneOver();

  static constexpr int playerMaxHp = 100;

  GameEngine &_ge;
  UIManager &_ui;
  IInputHandler &_input;
  // declared last, so the presenters are destroyed before anything they
  // reference
  SceneHud _hud;
};

#endif // TEXTQUESTCORE_H