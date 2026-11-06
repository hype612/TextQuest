#ifndef TEXTQUESTCORE_H
#define TEXTQUESTCORE_H

#include "Core.h"
#include "Difficulty.h"
#include "GameHud.h"
#include "IInputHandler.h"
#include "Menus.h"
#include "SceneBuilder.h"
#include "UIManager.h"
#include <memory>
#include <optional>

class TextQuestCore {
public:
  explicit TextQuestCore(GameEngine &ge);
  void run();

  TextQuestCore(const TextQuestCore &) = delete;
  TextQuestCore &operator=(const TextQuestCore &) = delete;
  TextQuestCore(TextQuestCore &&) = delete;
  TextQuestCore &operator=(TextQuestCore &&) = delete;

private:
  std::optional<unsigned int> loadLastSeed() const;
  void saveSeed(unsigned int seed) const;

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
  Menus _menus;
  // Per-scene. Declared last so it is destroyed first: its presenters observe
  // the scene's controllers and use _ui.
  std::unique_ptr<GameHud> _hud;
};

#endif // TEXTQUESTCORE_H
