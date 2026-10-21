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
#include <string>
#include <unordered_map>
#include <vector>

namespace {
// Flattens the generator's Tile grid into the engine's map string, then
// re-stamps each room's wall cells with a theme char picked per leaf room,
// so different rooms can use different wall textures.
std::string serializeThemedMap(const std::vector<std::vector<Tile>> &map,
                               const std::vector<Rect> &leafRooms,
                               const std::vector<char> &wallThemes) {
  if (wallThemes.empty()) {
    Logger::GetInstance()->log("serializeThemedMap: no wall themes given",
                               LogType::CORE, LogLevel::ERROR);
    Logger::GetInstance()->forceFlush();
    std::abort();
  }
  int mapHeight = static_cast<int>(map.size());
  int mapWidth = mapHeight > 0 ? static_cast<int>(map[0].size()) : 0;

  std::string out(static_cast<size_t>(mapWidth) * mapHeight, '.');
  for (int y = 0; y < mapHeight; y++)
    for (int x = 0; x < mapWidth; x++)
      if (map[y][x] == Tile::WALL)
        out[y * mapWidth + x] = wallThemes[0];

  for (const Rect &room : leafRooms) {
    char theme = wallThemes[rand() % wallThemes.size()];
    for (int y = 0; y < static_cast<int>(room.height); y++) {
      for (int x = 0; x < static_cast<int>(room.width); x++) {
        int gx = room.x + x, gy = room.y + y;
        if (map[gy][gx] == Tile::WALL)
          out[gy * mapWidth + gx] = theme;
      }
    }
  }
  return out;
}
} // namespace

// Loads every shading variant in `dir`, ordered so lv0 (brightest) is first.
std::vector<std::string> TextQuestCore::loadTextureVec(const std::string &dir) {
  std::unordered_map<std::string, std::string> texs;
  loadResources(dir, texs);
  std::vector<std::string> keys;
  keys.reserve(texs.size());
  for (const auto &[key, value] : texs)
    keys.push_back(key);
  std::sort(keys.begin(), keys.end());
  std::vector<std::string> out;
  out.reserve(texs.size());
  for (const auto &key : keys)
    out.push_back(texs[key]);
  Logger::GetInstance()->log(
      dir + " texture count: " + std::to_string(out.size()), LogType::TEXPREP,
      LogLevel::INFO);
  return out;
}

TextQuestCore::TextQuestCore(GameEngine &ge)
    : _ge(ge), _ui(ge.uiMan()), _input(ge.inputHandler()) {
  _ge.enableDistanceShading(true);
  std::vector<float> shadingThresholds;
  for (int i = 1; i < 8; i++)
    shadingThresholds.push_back(i * 2);
  _ge.setDistanceShadingThresholds(shadingThresholds);

  _ge.setOnSceneOver([this]() { onSceneOver(); });
  _ge.setPostSceneOver([this]() { onPostSceneOver(); });
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

TextQuestCore::BuiltScene TextQuestCore::buildScene() {
  Logger *l = Logger::GetInstance();
  auto scene = std::make_shared<SceneManager>();
  SceneManager &sceneMan = *scene;

  MapGenerator mg(Rect{0, 0, 50, 50}, part_params{.3f, .7f, 10, 10, 20, 20}, 5,
                  .35f, _seed);

  std::vector<std::string> walltexV =
      loadTextureVec("Resources/Textures/wall/");
  sceneMan.uploadTextureVecForWall('#', walltexV);
  std::vector<std::string> tnttexV = loadTextureVec("Resources/Textures/tnt/");
  sceneMan.uploadTextureVecForWall('T', tnttexV);

  std::string map = serializeThemedMap(mg.map(), mg.leafRooms(), {'#', 'T'});
  sceneMan.initializeNewMap(map, 50, 50);
  l->log("map inited", LogType::CORE, LogLevel::INFO);

  mg.generateSpawnPoints(/*max_enemy_density=*/0.0f, /*boss_room_count=*/1);
  vec2i playerSpawn{2, 2};
  vec2i istvanSpawn{10, 9};
  for (const SpawnPoint &sp : mg.spawnPoints()) {
    if (sp.type == SpawnType::PLAYER)
      playerSpawn = sp.pos;
    else if (sp.type == SpawnType::BOSS)
      istvanSpawn = sp.pos;
  }

  std::string intex = " \n";
  Transform init{
      {static_cast<float>(playerSpawn.x), static_cast<float>(playerSpawn.y)},
      0.f};
  l->log("before player construction", LogType::CORE, LogLevel::INFO);
  auto playerCtrl = std::make_unique<PlayerBehaviorController>(_input);
  PlayerBehaviorController *playerCtrlPtr = playerCtrl.get();
  Entity p(std::move(playerCtrl), init, &intex, playerMaxHp, 0.3f, true, 0.f,
           0.f);
  p.setMoveSpeedAllDirectons(3.5f);
  p.setTurnSpeedAlldirections(2.5f);
  sceneMan.AddEntity(p);
  l->log("player added to entities", LogType::CORE, LogLevel::INFO);

  // ==================
  // Istvan upload
  // ==================
  std::vector<std::string> IstvantexV =
      loadTextureVec("Resources/Textures/FeralGhoul/");
  vec2f istvanSpawnF{static_cast<float>(istvanSpawn.x),
                     static_cast<float>(istvanSpawn.y)};
  Transform IstvanInit{istvanSpawnF, 1.f};
  std::vector<vec2f> istvan_patrol_route =
      mg.generatePatrolPoints(istvanSpawnF, 3);

  auto istvanCtrl =
      std::make_unique<IstvanBehaviorController>(istvan_patrol_route, 1.f, 3.f);
  IstvanBehaviorController *istvanCtrlPtr = istvanCtrl.get();
  Entity Istvan(std::move(istvanCtrl), IstvanInit, IstvantexV, 100, 0.2f, false,
                180.f, 8.f);
  Istvan.setMoveSpeedAllDirectons(3.0f);
  Istvan.setTurnSpeedAlldirections(3.f);
  sceneMan.AddEntity(Istvan);

  // ==================
  // Win cond setup
  // ==================
  sceneMan.setWinCondition([](const SceneManager &sc_man) -> bool {
    return std::ranges::all_of(sc_man.entities(), [](const Entity &e) {
      return e.isPlayer() || e.health() <= 0;
    });
  });
  sceneMan.setCameraFollow(sceneMan.entityAtId(0).transform());
  sceneMan.setCameraFovDegrees(90);
  l->log("camera follow set", LogType::CORE, LogLevel::INFO);
  return {scene, playerCtrlPtr, istvanCtrlPtr};
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
      std::make_unique<HealthBarPresenter>(_ui, hbHandle, playerMaxHp);
  built.playerCtrl->setHealthObserver(hud.healthBar.get());

  // Istvan state debug display
  Rect stateArea{2, scr.y - 7, 30, 3};
  Rect stateWritable{1, 1, 28, 1};
  UIElementHandle stateHandle = _ui.addElement(UIElement(
      stateArea, stateWritable, IstvanStatePresenter::design(stateArea)));
  hud.istvanState = std::make_unique<IstvanStatePresenter>(_ui, stateHandle);
  built.istvanCtrl->setStateObserver(hud.istvanState.get());
  return hud;
}

// Builds a fresh scene plus its HUD and queues it on the engine. Also used
// for retrying: the old presenters and every overlay are dropped first, so
// the new HUD does not collide with the old overlays.
void TextQuestCore::startScene() {
  _hud = SceneHud{};
  _ui.clear();
  BuiltScene built = buildScene();
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

void TextQuestCore::loadResources(
    const std::string &filePath,
    std::unordered_map<std::string, std::string> &outTextures) {
  std::stringstream temp;
  std::string value;
  std::string key;
  for (const auto &entry : std::filesystem::directory_iterator(filePath)) {
    if (std::filesystem::is_regular_file(entry)) {
      std::ifstream current_file(entry.path());
      if (current_file.fail()) {
        std::cerr << "ERROR: failed to open file: " << entry.path().string()
                  << " reading next texture file..." << std::endl;
        continue;
      }

      temp.str(std::string());
      temp << current_file.rdbuf();
      key = entry.path().stem().string();
      if (outTextures.find(key) != outTextures.end()) {
        std::cerr << "ERROR: texture for this type of tile is already present. "
                     "Skipping this one...."
                  << std::endl;
        std::cerr << "texture: " << std::endl;
        std::cout << temp.str() << std::endl;
        continue;
      }
      value = temp.str();
      outTextures[key] = value;
      current_file.close();
    }
  }
}
