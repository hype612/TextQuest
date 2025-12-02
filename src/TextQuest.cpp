#include "Headers/Core.h"
#include "Headers/EngineState.h"
#include "Headers/SceneManager.h"
#include <fstream>
#include <string>
#include <unordered_map>

int main() {
  // logger init
  std::ofstream logFile("debug.log", std::ios::trunc);

  GameEngine ge;
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
  logFile << "before upload of walltex.." << std::endl;
  std::unordered_map<std::string, std::string> texs;
  sceneMan.loadResources("/home/attila/Kitchen/TextQuest/Resources/Textures/",
                         texs);
  // logFile << "wallTex size: " << texs["wall"].size();
  logFile << "after upload of walltex.." << std::endl;
  // right after sceneMan.loadResources(..., texs);
  logFile << "after loadResources\n";
  logFile.flush();

  for (const auto &p : texs) {
    logFile << "key in main: '" << p.first << "' size=" << p.second.size()
            << "\n";
  }
  logFile.flush();

  auto it = texs.find("wall");
  if (it == texs.end()) {
    logFile << "ERROR: wall key missing\n";
    logFile.flush();
  } else {
    // copy into local string before doing anything else
    std::string wallCopy = it->second;
    logFile << "wall found, copied size = " << wallCopy.size() << "\n";
    logFile.flush();
  }

  sceneMan.uploadTextureForWall('#', texs["wall"]);

  Player &p = sceneMan.getPlayerRef();
  p.set_x(5.f);
  p.set_y(5.f);
  ge.run_game();
}
