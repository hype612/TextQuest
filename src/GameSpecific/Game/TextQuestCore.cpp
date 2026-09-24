#include "TextQuestCore.h"
#include "Entity.h"
#include "Logger.h"
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

// Loads every shading variant in `dir`, ordered so lv0 (brightest) is first.
static std::vector<std::string> loadTextureVec(SceneManager &sceneMan,
                                               const std::string &dir) {
  std::unordered_map<std::string, std::string> texs;
  sceneMan.loadResources(dir, texs);
  std::vector<std::string> keys;
  keys.reserve(texs.size());
  for (const auto &[key, value] : texs)
    keys.push_back(key);
  std::sort(keys.begin(), keys.end());
  std::vector<std::string> out;
  out.reserve(texs.size());
  for (const auto &key : keys)
    out.push_back(texs[key]);
  Logger::GetInstance()->log(dir + " texture count: " +
                                 std::to_string(out.size()),
                             LogType::TEXPREP, LogLevel::INFO);
  return out;
}

TextQuestCore::TextQuestCore(GameEngine &ge)
    : _ge(ge), _ui(ge.uiMan()), _input(ge.inputHandler()) {
  Logger *l = Logger::GetInstance();
  startScene();
  l->log("scene set", LogType::CORE, LogLevel::INFO);

  _ge.enableDistanceShading(true);
  std::vector<float> shadingThresholds;
  for (int i = 1; i < 8; i++)
    shadingThresholds.push_back(i * 2);
  _ge.setDistanceShadingThresholds(shadingThresholds);

  _ge.setOnSceneOver([this]() { onSceneOver(); });
  _ge.setPostSceneOver([this]() { onPostSceneOver(); });
}

void TextQuestCore::run() {
  Logger::GetInstance()->log("all prep is done, now running game..",
                             LogType::CORE, LogLevel::INFO);
  _ge.run_game();
}

TextQuestCore::BuiltScene TextQuestCore::buildScene() {
  Logger *l = Logger::GetInstance();
  auto scene = std::make_shared<SceneManager>();
  SceneManager &sceneMan = *scene;
  std::string map = "";
  map += "################################";
  map += "#..............................#";
  map += "#...####..............####.....#";
  map += "#...#..#..............#..#.....#";
  map += "#...####....##..##....####.....#";
  map += "#...........##..##.............#";
  map += "#..............................#";
  map += "#............T.................#";
  map += "#.............T................#";
  map += "#..............................#";
  map += "#.......#..............#.......#";
  map += "#.......#......##......#.......#";
  map += "#.......########.......#.......#";
  map += "#....#####.............#.......#";
  map += "#................#######.......#";
  map += "#....#.T.#.......#.............#";
  map += "#....#...#.......#.#######.....#";
  map += "#....#####.......#.#...........#";
  map += "################################";

  sceneMan.initializeNewMap(map, 32, 19);
  l->log("map inited", LogType::CORE, LogLevel::INFO);
  std::vector<std::string> walltexV =
      loadTextureVec(sceneMan, "Resources/Textures/wall/");
  sceneMan.uploadTextureVecForWall('#', walltexV);
  std::vector<std::string> tnttexV =
      loadTextureVec(sceneMan, "Resources/Textures/tnt/");
  sceneMan.uploadTextureVecForWall('T', tnttexV);

  std::string intex = " \n";
  Transform init{{2.f, 2.f}, 0.f};
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
      loadTextureVec(sceneMan, "Resources/Textures/FeralGhoul/");
  Transform IstvanInit{{10.f, 9.f}, 1.f};
  std::vector<vec2f> istvan_patrol_route{{10.f, 9.f}, {20.f, 9.f}, {20.f, 6.f}};

  auto istvanCtrl =
      std::make_unique<IstvanBehaviorController>(istvan_patrol_route);
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
    int pad = (static_cast<int>(deathWritable.width) -
               static_cast<int>(msg.size())) /
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