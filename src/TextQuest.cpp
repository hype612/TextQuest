#include "Core.h"
#include "GameSpecific/Game/TextQuestCore.h"
#include <cstdlib>
#include <exception>
#include <iostream>

static void runGame() {
  GameEngine ge;
  // declared after ge, so it is destroyed first(presenters before UIManager)
  TextQuestCore game(ge);
  game.run();
}

int main() {
  // The engine is destroyed (restoring the terminal) before the handler
  // runs, so stderr is usable here.
  try {
    runGame();
  } catch (const std::exception &e) {
    std::cerr << "TextQuest: fatal error: " << e.what() << '\n';
    return EXIT_FAILURE;
  }
}
