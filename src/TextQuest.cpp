#include "Headers/Core.h"
#include "Headers/EngineState.h"
#include "Headers/Logger.h"
#include "Headers/MoveDirection.h"
#include "Headers/SceneManager.h"
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>

int main() {
  auto ge_ptr = std::make_unique<GameEngine>();
  SceneManager &sceneMan = *(EngineState::GetInstance()->globalSceneManager);
  std::string map = "";
  /*
  map += "##################";
  map += "#................#";
  map += "#................#";
  map += "#........##......#";
  map += "#.......##.......#";
  map += "#................#";
  map += "#................#";
  map += "##################";
  */
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
  sceneMan.uploadTextureVecForWall('T', tnttexV);

  Player &p = sceneMan.getPlayerRef();
  p.setX(2.f);
  p.setY(2.f);
  p.setAngle(0.f);
  p.setMoveSpeedAllDirectons(5.f);
  p.setTurnSpeedAlldirections(5.f);
  ge_ptr->enableDistanceShading(true);
  std::vector<float> shadingThresholds;
  for (int i = 1; i < 8; i++)
    shadingThresholds.push_back(i * 2);
  ge_ptr->setDistanceShadingThresholds(shadingThresholds);
  ge_ptr->run_game();
}
