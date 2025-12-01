#include "Headers/Core.h"
#include "Headers/EngineState.h"
#include "Headers/SceneManager.h"
#include <fstream>
#include <string>
#include <unordered_map>

int main() {
  // logger init
  std::ofstream logFile("debug.log", std::ios::app);

  GameEngine ge;
  SceneManager &sceneMan = *(EngineState::GetInstance()->globalSceneManager);

  std::wstring map = L"";
  map += L"##################";
  map += L"#................#";
  map += L"#................#";
  map += L"#........##......#";
  map += L"#.......##.......#";
  map += L"#................#";
  map += L"#................#";
  map += L"##################";
  sceneMan.initializeNewMap(map, 18, 8);
  logFile << "before upload of walltex.." << std::endl;
  std::unordered_map<std::string, std::wstring> texs = sceneMan.loadResources(
      "/home/attila/Kitchen/TextQuest/Resources/Textures/");
  logFile << "wallTex size: " << texs["wall"].size();
  sceneMan.uploadTextureForWall(L'#', texs["wall"]);

  Player &p = sceneMan.getPlayerRef();
  p.set_x(5.f);
  p.set_y(5.f);
  ge.run_game();
}
