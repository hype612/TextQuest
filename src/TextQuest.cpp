#include "Headers/Core.h"
#include "Headers/EngineState.h"
#include "Headers/Logger.h"
#include "Headers/SceneManager.h"
#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>

int main() {
  auto ge_ptr = std::make_unique<GameEngine>();
  SceneManager &sceneMan = *(EngineState::GetInstance()->globalSceneManager);
  Logger::setLogLevel(LogLevel::WARNING);
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
  Logger::GetInstance()->log("reading walltex...", LogType::TEXPREP,
                             LogLevel::INFO);
  Logger::forceFlush();
  sceneMan.loadResources(
      "/home/attila/Kitchen/TextQuest/Resources/Textures/wall/", texs);
  Logger::GetInstance()->log("read walltex folder. (COMPLETE)",
                             LogType::TEXPREP, LogLevel::INFO);
  Logger::forceFlush();
  std::vector<std::string> walltexV;
  Logger::GetInstance()->log("converting map to vector...", LogType::TEXPREP,
                             LogLevel::INFO);
  Logger::forceFlush();
  std::vector<std::string> keys;
  keys.reserve(texs.size());
  for (const auto &[key, value] : texs)
    keys.push_back(key);
  std::sort(keys.begin(), keys.end());
  walltexV.reserve(texs.size());
  for (const auto &key : keys) {
    walltexV.push_back(texs[key]);
  }
  Logger::GetInstance()->log("conversion DONE", LogType::TEXPREP,
                             LogLevel::INFO);
  Logger::forceFlush();
  // sceneMan.uploadTextureForWall('#', texs["wall"]);
  Logger::GetInstance()->log("all done, textures read. Uploading it for wall.",
                             LogType::TEXPREP, LogLevel::INFO);
  Logger::forceFlush();
  sceneMan.uploadTextureVecForWall('#', walltexV);
  Logger::GetInstance()->log("tex uploaded for wall", LogType::TEXPREP,
                             LogLevel::INFO);
  Logger::forceFlush();
  Player &p = sceneMan.getPlayerRef();
  p.set_x(2.f);
  p.set_y(2.f);
  p.set_angle(.0f);
  ge_ptr->enableDistanceShading(true);
  std::vector<float> shadingThresholds;
  for (int i = 1; i < 8; i++)
    shadingThresholds.push_back(i * 2);
  ge_ptr->setDistanceShadingThresholds(shadingThresholds);
  Logger::GetInstance()->log("starting game", LogType::TEXPREP, LogLevel::INFO);
  ge_ptr->run_game();
}
