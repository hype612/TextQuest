#include "TextQuestCore.h"
#include "Logger.h"
#include <ctime>
#include <fstream>
#include <string>
#include <vector>

TextQuestCore::TextQuestCore(GameEngine &ge)
    : _ge(ge), _ui(ge.uiMan()), _input(ge.inputHandler()),
      _builder(_input, "Resources/Textures/"), _menus(_ge, _ui, _input) {
  _ge.enableDistanceShading(true);
  std::vector<float> shadingThresholds;
  for (int i = 1; i < 8; i++)
    shadingThresholds.push_back(i * 2);
  _ge.setDistanceShadingThresholds(shadingThresholds);

  _ge.setOnSceneOver([this]() { onSceneOver(); });
  _ge.setPostSceneOver([this]() { onPostSceneOver(); });
  _ge.setOnFrame([this](float dt) {
    if (_hud)
      _hud->update(dt);
  });

  _builder.registerEnemyArchetype(
      "ghoul",
      EnemyArchetype{_builder.loadTextureVec("FeralGhoul"), 100, 10.f, 0.2f,
                     0.2f, 180.f, 8.f, 3.f, 3.f, 1.f, 0.8f, 0.3f});
  _builder.registerEnemyArchetype(
      "demon", EnemyArchetype{_builder.loadTextureVec("Cyberdemon"), 500, 25.f,
                              0.35f, 0.35f, 180.f, 12.f, 2.5f, 3.f, 0.7f, 0.5f,
                              0.15f});
}

void TextQuestCore::run() {
  Logger *l = Logger::GetInstance();
  std::optional<unsigned int> savedSeed = loadLastSeed();
  Menus::MenuChoice choice = _menus.showStart(savedSeed);
  if (choice == Menus::MenuChoice::EXIT)
    return;

  std::optional<Difficulty> difficulty = _menus.showDifficulty();
  if (!difficulty)
    return;
  _chosenDifficulty = *difficulty;

  _seed = (choice == Menus::MenuChoice::LAST_SEED && savedSeed)
              ? *savedSeed
              : static_cast<unsigned int>(std::time(nullptr));
  saveSeed(_seed);
  l->log("map seed: " + std::to_string(_seed), LogType::CORE, LogLevel::INFO);

  startScene();
  l->log("all prep is done, now running game..", LogType::CORE, LogLevel::INFO);
  _ge.run_game();
}

std::optional<unsigned int> TextQuestCore::loadLastSeed() const {
  std::ifstream file(seedFile);
  unsigned int seed;
  if (file >> seed)
    return seed;
  return std::nullopt;
}

void TextQuestCore::saveSeed(unsigned int seed) const {
  std::ofstream file(seedFile);
  file << seed << '\n';
  if (!file)
    Logger::GetInstance()->log(std::string("could not write ") + seedFile,
                               LogType::CORE, LogLevel::WARNING);
}

void TextQuestCore::startScene() {
  _hud.reset();
  _menus.hideEnd();
  _ui.clear();
  SceneBuilder::BuiltScene built = _builder.build(_seed, _chosenDifficulty);
  _hud = std::make_unique<GameHud>(_ui, _ge.screenSize(), built,
                                   _builder.playerMaxHp());
  _ge.setScene(built.scene);
}

void TextQuestCore::onSceneOver() {
  _menus.showEnd(_ge.sceneMan().playerWon());
}

void TextQuestCore::onPostSceneOver() {
  if (_input.rawKeyPressed('r'))
    startScene();
}
