#include "Core.h"
#include "GameSpecific/Game/TextQuestCore.h"

int main() {
  GameEngine ge;
  // declared after ge, so it is destroyed first (presenters before UIManager)
  TextQuestCore game(ge);
  game.run();
}
