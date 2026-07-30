#include "Entity.h"
#include "GameSpecific/Headers/PlayerBehaviorController.h"
#include "Headers/Core.h"
#include "Headers/Logger.h"
#include "Headers/MoveDirection.h"
#include "Headers/SceneManager.h"
#include "player.h"
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
  map += "#.......#..............#.......#";
  map += "#.......#......##......#.......#";
  map += "#.......########.......#.......#";
  map += "#....#####.............#.......#";
  map += "#................#######.......#";
  map += "#....#.T.#.......#.............#";
  map += "#....#...#.......#.#######.....#";
  map += "#....#####.......#.#...........#";
  map += "################################";

  sceneMan.initializeNewMap(map, 32, 16);
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
  tkeys.reserve(wtexs.size());
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
           init, &intex, 100);
  l->log("player created", LogType::CORE, LogLevel::INFO);
  p.setMoveSpeedAllDirectons(5.f);
  p.setTurnSpeedAlldirections(5.f);
  l->log("movement speeds set", LogType::CORE, LogLevel::INFO);
  sceneMan.AddEntity(p);
  l->log("player added to entities", LogType::CORE, LogLevel::INFO);
  l->log("all prep is done, now running game..", LogType::CORE, LogLevel::INFO);
  sceneMan.setCameraFollow(sceneMan.entityAtId(0).transform());
  sceneMan.setCameraFovDegrees(90);
  l->log("camera follow set", LogType::CORE, LogLevel::INFO);
  ge_ptr->run_game();
}
