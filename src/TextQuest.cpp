#include "Headers/Core.h"
#include "Headers/EngineState.h"
#include "Headers/SceneManager.h"
#include <string>
#include <unordered_map>

int main() {
  GameEngine ge;
  SceneManager sceneMan = *(EngineState::GetInstance()->globalSceneManager);

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

  std::unordered_map<std::string, std::wstring> texs = sceneMan.loadResources(
      "/home/attila/Kitchen/TextQuest/Resources/Textures/");
  sceneMan.uploadTextureForWall(L'#', texs["wall"]);

  Player &p = sceneMan.getPlayerRef();
  p.set_x(1);
  p.set_y(1);
  ge.run_game();
}
