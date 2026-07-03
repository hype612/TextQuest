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
  map += "##################";
  map += "#................#";
  map += "#................#";
  map += "#........##......#";
  map += "#.......##.......#";
  map += "#................#";
  map += "#................#";
  map += "##################";
  sceneMan.initializeNewMap(map, 18, 8);
  std::unordered_map<std::string, std::string> texs;
  // sceneMan.loadResources("/home/attila/Kitchen/Resources/Textures/", texs);
  sceneMan.loadResources(
      "/home/attila/Kitchen/TextQuest/Resources/Textures/wall/", texs);
  std::vector<std::string> walltexV;
  std::vector<std::string> keys;
  keys.reserve(texs.size());
  for (const auto &[key, value] : texs)
    keys.push_back(key);
  std::sort(keys.begin(), keys.end());
  walltexV.reserve(texs.size());
  for (const auto &key : keys) {
    walltexV.push_back(texs[key]);
  }
  // sceneMan.uploadTextureForWall('#', texs["wall"]);
  sceneMan.uploadTextureVecForWall('#', walltexV);
  Player &p = sceneMan.getPlayerRef();
  p.setX(2.f);
  p.setY(2.f);
  p.setAngle(.0f);
  p.setMoveSpeedAllDirectons(15.f);
  p.setTurnSpeedAlldirections(5.f);
  ge_ptr->enableDistanceShading(true);
  std::vector<float> shadingThresholds;
  for (int i = 1; i < 8; i++)
    shadingThresholds.push_back(i * 2);
  ge_ptr->setDistanceShadingThresholds(shadingThresholds);
  ge_ptr->run_game();
}
