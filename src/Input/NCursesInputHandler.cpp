#include "../Headers/NCursesInputHandler.h"
#include <unordered_set>

#if (defined(LINUX) || defined(__linux__))

NCursesInputHandler::NCursesInputHandler(Player &player)
    : IInputHandler(player) {}

void NCursesInputHandler::Init() {
  // check if ncurses has been inited
  // it should be, since renderer is inited first
}

void NCursesInputHandler::ReceiveMovementInput(float delta) {
  std::unordered_set<int> keysPressed;

  int key = getch();
  while (key != ERR) {
    keysPressed.insert(key);
    key = getch();
  }
  if (keysPressed.count('a') || keysPressed.count('A')) {
    _player.move(delta, MoveDirection::STRAFE_LEFT);
  }
  if (keysPressed.count('d') || keysPressed.count('D')) {
    _player.move(delta, MoveDirection::STRAFE_RIGHT);
  }
  if (keysPressed.count('w') || keysPressed.count('W')) {
    _player.move(delta, MoveDirection::FORWARD);
  }
  if (keysPressed.count('s') || keysPressed.count('S')) {
    _player.move(delta, MoveDirection::BACKWARD);
  }
  if (keysPressed.count('q') || keysPressed.count('Q')) {
    _player.move(delta, MoveDirection::TURN_LEFT);
  }
  if (keysPressed.count('e') || keysPressed.count('E')) {
    _player.move(delta, MoveDirection::TURN_RIGHT);
  }
}

#endif // OS Check end
