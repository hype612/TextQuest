#include "Entity.h"
#include "GameSpecific/Headers/IstvanBehaviorController.h"
#include "GameSpecific/Headers/PlayerBehaviorController.h"
#include "Headers/Core.h"
#include "Headers/Logger.h"
#include "Headers/MoveDirection.h"
#include "Headers/SceneManager.h"
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>

int main() {
  Logger *l = Logger::GetInstance();
  auto ge_ptr = std::make_unique<GameEngine>();
  SceneManager &sceneMan = ge_ptr->sceneMan();
  l->log("ge_ptr good, sceneman returned", LogType::CORE, LogLevel::INFO);
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
  std::unordered_map<std::string, std::string> wtexs;
  l->log("map inited", LogType::CORE, LogLevel::INFO);
  // sceneMan.loadResources("/home/attila/Kitchen/Resources/Textures/", texs);
  sceneMan.loadResources(
      "/home/attila/Kitchen/TextQuest/Resources/Textures/wall/", wtexs);
  std::vector<std::string> walltexV;
  std::vector<std::string> keys;
  keys.reserve(wtexs.size());
  for (const auto &[key, value] : wtexs)
    keys.push_back(key);
  std::sort(keys.begin(), keys.end());
  walltexV.reserve(wtexs.size());
  for (const auto &key : keys) {
    walltexV.push_back(wtexs[key]);
  }
  // sceneMan.uploadTextureForWall('#', texs["wall"]);
  l->log("walltexV size: " + std::to_string(walltexV.size()), LogType::TEXPREP,
         LogLevel::INFO);
  sceneMan.uploadTextureVecForWall('#', walltexV);

  std::unordered_map<std::string, std::string> ttexs;
  // sceneMan.loadResources("/home/attila/Kitchen/Resources/Textures/", texs);
  sceneMan.loadResources(
      "/home/attila/Kitchen/TextQuest/Resources/Textures/tnt/", ttexs);
  std::vector<std::string> tnttexV;
  std::vector<std::string> tkeys;
  tkeys.reserve(ttexs.size());
  for (const auto &[key, value] : ttexs)
    tkeys.push_back(key);
  std::sort(tkeys.begin(), tkeys.end());
  tnttexV.reserve(ttexs.size());
  for (const auto &key : tkeys) {
    tnttexV.push_back(ttexs[key]);
  }
  // sceneMan.uploadTextureForWall('#', texs["wall"]);
  l->log("tnttexV size: " + std::to_string(tnttexV.size()), LogType::TEXPREP,
         LogLevel::INFO);
  sceneMan.uploadTextureVecForWall('T', tnttexV);

  ge_ptr->enableDistanceShading(true);
  std::vector<float> shadingThresholds;
  for (int i = 1; i < 8; i++)
    shadingThresholds.push_back(i * 2);
  ge_ptr->setDistanceShadingThresholds(shadingThresholds);
  l->log("uploaded all textures", LogType::CORE, LogLevel::INFO);

  std::string intex = " \n";
  Transform init{{2.f, 2.f}, 0.f};
  l->log("before player construction", LogType::CORE, LogLevel::INFO);
  Entity p(std::make_unique<PlayerBehaviorController>(ge_ptr->inputHandler()),
           init, &intex, 100, .0f, true, 0.f, 0.f);
  p.setMoveSpeedAllDirectons(3.5f);
  p.setTurnSpeedAlldirections(2.5f);
  sceneMan.AddEntity(p);
  l->log("player added to entities", LogType::CORE, LogLevel::INFO);

  // ==================
  // Istvan upload
  // ==================

  std::unordered_map<std::string, std::string> Istvantexs;
  sceneMan.loadResources(
      "/home/attila/Kitchen/TextQuest/Resources/Textures/FeralGhoul/",
      Istvantexs);
  std::vector<std::string> IstvantexV;
  std::vector<std::string> Istvankeys;
  Istvankeys.reserve(Istvantexs.size());
  for (const auto &[key, value] : Istvantexs)
    Istvankeys.push_back(key);
  std::sort(Istvankeys.begin(), Istvankeys.end());
  IstvantexV.reserve(Istvantexs.size());
  for (const auto &key : Istvankeys) {
    IstvantexV.push_back(Istvantexs[key]);
  }
  Transform IstvanInit{{10.f, 9.f}, 0.f};
  Entity Istvan(std::make_unique<IstvanBehaviorController>(), IstvanInit,
                IstvantexV, 100, 0.2f, false, 360.f, 8.f);
  Istvan.setMoveSpeedAllDirectons(.0f);
  Istvan.setTurnSpeedAlldirections(3.f);
  sceneMan.AddEntity(Istvan);

  // ==================
  // Bela upload
  // ==================

  std::unordered_map<std::string, std::string> Belatexs;
  sceneMan.loadResources(
      "/home/attila/Kitchen/TextQuest/Resources/Textures/Cyberdemon/",
      Belatexs);
  std::vector<std::string> BelatexV;
  std::vector<std::string> Belakeys;
  Belakeys.reserve(Belatexs.size());
  for (const auto &[key, value] : Belatexs)
    Belakeys.push_back(key);
  std::sort(Belakeys.begin(), Belakeys.end());
  BelatexV.reserve(Belatexs.size());
  for (const auto &key : Belakeys) {
    BelatexV.push_back(Belatexs[key]);
  }
  Transform BelaInit{{12.f, 10.f}, 0.f};
  Entity Bela(std::make_unique<IstvanBehaviorController>(), BelaInit, BelatexV,
              30, .35f, false, 1.f, 1.f);
  Bela.setMoveSpeedAllDirectons(.0f);
  Bela.setTurnSpeedAlldirections(1.5f);
  sceneMan.AddEntity(Bela);

  l->log("all prep is done, now running game..", LogType::CORE, LogLevel::INFO);
  sceneMan.setCameraFollow(sceneMan.entityAtId(0).transform());
  sceneMan.setCameraFovDegrees(90);
  l->log("camera follow set", LogType::CORE, LogLevel::INFO);
  ge_ptr->run_game();
}
