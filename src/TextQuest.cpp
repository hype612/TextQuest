#include "Headers/Core.h"
#include "Headers/EngineState.h"
#include "Headers/SceneManager.h"
#include <memory>
#include <string>
#include <unordered_map>

int main() {
  // logger init
  // std::ofstream logFile("debug.log", std::ios::trunc);
  auto ge_ptr = std::make_unique<GameEngine>();
  // GameEngine ge;
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
  // logFile << "before upload of walltex.." << std::endl;
  std::unordered_map<std::string, std::string> texs;
  sceneMan.loadResources("/home/attila/Kitchen/TextQuest/Resources/Textures/",
                         texs);
  // logFile << "wallTex size: " << texs["wall"].size();
  // logFile << "after upload of walltex.." << std::endl;
  // right after sceneMan.loadResources(..., texs);
  // logFile << "after loadResources\n";
  // logFile.flush();

  // logFile.flush();
  sceneMan.uploadTextureForWall('#', texs["wall"]);
  // logFile << sceneMan.getWallTextureForMapChar('#') << std::endl;
  Player &p = sceneMan.getPlayerRef();
  p.set_x(2.f);
  p.set_y(2.f);
  p.set_angle(.0f);
  ge_ptr->run_game();
}
