#include "Headers/Core.h"
#include "Headers/EngineState.h"
#include "Headers/Logger.h"
#include "Headers/SceneManager.h"
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
  sceneMan.loadResources("/home/attila/Kitchen/Resources/Textures/", texs);

  sceneMan.uploadTextureForWall('#', texs["wall"]);
  Player &p = sceneMan.getPlayerRef();
  p.set_x(2.f);
  p.set_y(2.f);
  p.set_angle(.0f);
  ge_ptr->run_game();
}
