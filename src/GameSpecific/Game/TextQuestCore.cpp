#include "TextQuestCore.h"
#include "../MapGen/MapGenerator.h"
#include "Difficulty.h"
#include "Entity.h"
#include "Logger.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

TextQuestCore::TextQuestCore(GameEngine &ge)
    : _ge(ge), _ui(ge.uiMan()), _input(ge.inputHandler()),
      _builder(_input, "Resources/Textures/") {
  _ge.enableDistanceShading(true);
  std::vector<float> shadingThresholds;
  for (int i = 1; i < 8; i++)
    shadingThresholds.push_back(i * 2);
  _ge.setDistanceShadingThresholds(shadingThresholds);

  _ge.setOnSceneOver([this]() { onSceneOver(); });
  _ge.setPostSceneOver([this]() { onPostSceneOver(); });
  _ge.setOnFrame([this](float dt) {
    if (_hud.minimap)
      _hud.minimap->update();
    if (_hud.viewmodel)
      _hud.viewmodel->update(dt);
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
  MenuChoice choice = showMenu(savedSeed);
  if (choice == MenuChoice::EXIT)
    return;

  if (!showDifficultyMenu())
    return;

  _seed = (choice == MenuChoice::LAST_SEED && savedSeed)
              ? *savedSeed
              : static_cast<unsigned int>(std::time(nullptr));
  saveSeed(_seed);
  l->log("map seed: " + std::to_string(_seed), LogType::CORE, LogLevel::INFO);

  startScene();
  l->log("all prep is done, now running game..", LogType::CORE, LogLevel::INFO);
  _ge.run_game();
}

TextQuestCore::MenuChoice
TextQuestCore::showMenu(std::optional<unsigned int> savedSeed) {
  vec2i scr = _ge.screenSize();
  const unsigned int w = 36;
  const unsigned int h = 7;
  Rect area{(scr.x - static_cast<int>(w)) / 2,
            (scr.y - static_cast<int>(h)) / 2, w, h};
  Rect writable{1, 1, w - 2, h - 2};
  std::vector<std::string> design;
  design.push_back(std::string(w, '='));
  for (unsigned int i = 0; i < h - 2; i++)
    design.push_back("|" + std::string(w - 2, ' ') + "|");
  design.push_back(std::string(w, '='));

  std::string lastSeedLine =
      savedSeed ? "  2) Replay last seed: " + std::to_string(*savedSeed)
                : "  2) Replay last seed (none saved)";
  UIElementHandle handle = _ui.addElement(UIElement(area, writable, design));
  if (UIElement *el = _ui.elementAt(handle))
    el->updateContent({"  TEXTQUEST", "", "  1) New game (random seed)",
                       lastSeedLine, "  3) Exit"});

  MenuChoice choice = MenuChoice::EXIT;
  _ge.runMenu([&]() {
    if (_input.rawKeyPressed('1')) {
      choice = MenuChoice::NEW_GAME;
      return false;
    }
    if (savedSeed && _input.rawKeyPressed('2')) {
      choice = MenuChoice::LAST_SEED;
      return false;
    }
    if (_input.rawKeyPressed('3') || _input.rawKeyPressed('x'))
      return false;
    return true;
  });
  _ui.removeElement(handle);
  return choice;
}

bool TextQuestCore::showDifficultyMenu() {
  vec2i scr = _ge.screenSize();
  const unsigned int w = 36;
  const unsigned int h = 7;
  Rect area{(scr.x - static_cast<int>(w)) / 2,
            (scr.y - static_cast<int>(h)) / 2, w, h};
  Rect writable{1, 1, w - 2, h - 2};
  std::vector<std::string> design;
  design.push_back(std::string(w, '='));
  for (unsigned int i = 0; i < h - 2; i++)
    design.push_back("|" + std::string(w - 2, ' ') + "|");
  design.push_back(std::string(w, '='));

  UIElementHandle handle = _ui.addElement(UIElement(area, writable, design));
  if (UIElement *el = _ui.elementAt(handle))
    el->updateContent(
        {"  SELECT DIFFICULTY", "", "  1) Easy", "  2) Normal", "  3) Hard"});

  bool picked = false;
  _ge.runMenu([&]() {
    if (_input.rawKeyPressed('1')) {
      _chosenDifficulty = EASY;
      picked = true;
      return false;
    }
    if (_input.rawKeyPressed('2')) {
      _chosenDifficulty = NORMAL;
      picked = true;
      return false;
    }
    if (_input.rawKeyPressed('3')) {
      _chosenDifficulty = HARD;
      picked = true;
      return false;
    }
    if (_input.rawKeyPressed('x'))
      return false;
    return true;
  });
  _ui.removeElement(handle);
  return picked;
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

TextQuestCore::SceneHud TextQuestCore::buildHud(BuiltScene &built) {
  SceneHud hud;
  vec2i scr = _ge.screenSize();

  // Health bar
  Rect hbArea{1, scr.y - 3, 50, 3};
  Rect hbWritable{1, 1, 48, 1};
  UIElementHandle hbHandle = _ui.addElement(
      UIElement(hbArea, hbWritable, HealthBarPresenter::design(hbArea)));
  hud.healthBar =
      std::make_unique<HealthBarPresenter>(_ui, hbHandle,
                                           _builder.playerMaxHp());

  // Demon state debug display
  Rect stateArea{2, scr.y - 7, 30, 3};
  Rect stateWritable{1, 1, 28, 1};
  UIElementHandle stateHandle = _ui.addElement(UIElement(
      stateArea, stateWritable, IstvanStatePresenter::design(stateArea)));
  hud.istvanState = std::make_unique<IstvanStatePresenter>(_ui, stateHandle);
  built.istvanCtrl->setStateObserver(hud.istvanState.get());

  // Minimap, top-right corner
  const int mmW = 25, mmH = 13;
  Rect mmArea{scr.x - mmW - 1, 1, mmW, mmH};
  Rect mmWritable{1, 1, mmW - 2, mmH - 2};
  UIElementHandle mmHandle = _ui.addElement(
      UIElement(mmArea, mmWritable, MinimapPresenter::design(mmArea)));
  hud.minimap = std::make_unique<MinimapPresenter>(
      _ui, mmHandle, *built.scene, built.scene->entityAtId(0).transform());

  // Damage feedback: the player has a single health observer, so fan out to
  // the bar and the flash.
  hud.damageFlash = std::make_unique<DamageFlashPresenter>(
      _ui, std::vector<UIElementHandle>{hbHandle, stateHandle, mmHandle},
      _builder.playerMaxHp());
  hud.healthFanOut = std::make_unique<HealthObserverFanOut>();
  hud.healthFanOut->add(hud.healthBar.get());
  hud.healthFanOut->add(hud.damageFlash.get());
  built.playerCtrl->setHealthObserver(hud.healthFanOut.get());

  // Weapon viewmodel, bottom-centre
  UIElementHandle vmHandle = _ui.addElement(ViewmodelPresenter::makeElement(
      ViewmodelPresenter::area(scr.x, scr.y)));
  hud.viewmodel = std::make_unique<ViewmodelPresenter>(_ui, vmHandle);
  built.playerCtrl->setShootObserver(hud.viewmodel.get());
  return hud;
}

// Builds a fresh scene plus its HUD and queues it on the engine. Also used
// for retrying: the old presenters and every overlay are dropped first, so
// the new HUD does not collide with the old overlays.
void TextQuestCore::startScene() {
  _hud = SceneHud{};
  _ui.clear();
  BuiltScene built = _builder.build(_seed, _chosenDifficulty);
  _hud = buildHud(built);
  _ge.setScene(built.scene);
}

void TextQuestCore::onSceneOver() {
  vec2i scr = _ge.screenSize();
  const unsigned int w = 20;
  const unsigned int h = 3;
  Rect deathArea{(scr.x - static_cast<int>(w)) / 2,
                 (scr.y - static_cast<int>(h)) / 2, w, h};
  Rect deathWritable{1, 1, w - 2, 1};
  std::vector<std::string> design = {std::string(w, '='),
                                     "|" + std::string(w - 2, ' ') + "|",
                                     std::string(w, '=')};
  UIElementHandle deathHandle =
      _ui.addElement(UIElement(deathArea, deathWritable, design));
  UIElement *el = _ui.elementAt(deathHandle);
  if (el) {
    std::string msg = _ge.sceneMan().playerWon()
                          ? "YOU WON. Retry? (r=yes, x=quit)"
                          : "YOU DIED. Retry? (r=yes, x=quit)";
    int pad =
        (static_cast<int>(deathWritable.width) - static_cast<int>(msg.size())) /
        2;
    if (pad < 0)
      pad = 0;
    el->updateContent({std::string(pad, ' ') + msg});
  }
}

void TextQuestCore::onPostSceneOver() {
  if (_input.rawKeyPressed('r'))
    startScene();
}
