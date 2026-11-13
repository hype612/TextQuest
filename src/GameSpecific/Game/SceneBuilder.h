#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include "../Headers/EnemyBehaviorController.h"
#include "../Headers/PlayerBehaviorController.h"
#include "../MapGen/cellularAutomata.hpp"
#include "Difficulty.h"
#include "EnemyArchetype.h"
#include "SceneManager.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class IInputHandler;

// Builds a playable scene: map, player, enemies and win condition. Owns the
// enemy archetype table. Stateless between builds, so retrying is a new call.
class SceneBuilder {
public:
  // The controllers are owned by their entities inside the scene. The raw
  // pointers are handed out so the HUD can attach presenters to them.
  struct BuiltScene {
    std::shared_ptr<SceneManager> scene;
    PlayerBehaviorController *playerCtrl;
    EnemyBehaviorController *enemyCtrl;
  };

  // `textureRoot` is the directory that holds one subdirectory per texture.
  SceneBuilder(IInputHandler &input, std::string textureRoot);

  BuiltScene build(unsigned int seed, const Difficulty &difficulty);

  int playerMaxHp() const { return _playerMaxHp; }

  // Duplicate keys throw.
  void registerEnemyArchetype(std::string name, EnemyArchetype type);
  // Loads every shading variant of the texture in <textureRoot>/<name>,
  // ordered so lv0 (brightest) is first.
  std::vector<std::string> loadTextureVec(const std::string &name) const;

private:
  // Builds the controller of a new enemy from its difficulty-scaled archetype.
  using ControllerFactory = std::function<std::unique_ptr<IBehaviorController>(
      const EnemyArchetype &)>;

  void spawnEnemy(const std::string &typeKey, SceneManager &scene, vec2f pos,
                  const Difficulty &difficulty,
                  const ControllerFactory &makeCtrl);
  std::string serializeThemedMap(const std::vector<std::vector<Tile>> &map,
                                 const std::vector<Rect> &leafRooms,
                                 const std::vector<char> &wallThemes) const;

  // Reads every regular file of `dir` into a stem -> contents map.
  std::unordered_map<std::string, std::string>
  readTextureDir(const std::string &dir) const;

  IInputHandler &_input;
  const std::string _textureRoot;
  const int _playerMaxHp = 100;
  std::unordered_map<std::string, EnemyArchetype> _enemyArchetypes;
};

#endif // SCENEBUILDER_H
