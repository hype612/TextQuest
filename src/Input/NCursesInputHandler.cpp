#include "../Headers/NCursesInputHandler.h"

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
    _player.subtractf_angle(10.f * delta);
  }
  if (keysPressed.count('d') || keysPressed.count('D')) {
    _player.addto_angle(10.f * delta);
  }
  if (keysPressed.count('w') || keysPressed.count('W')) {
    _player.addto_x(sinf(_player.get_angle()) * 45.0f * delta);
    _player.addto_y(cosf(_player.get_angle()) * 45.0f * delta);
  }
  if (keysPressed.count('s') || keysPressed.count('S')) {
    _player.subtractf_x(sinf(_player.get_angle()) * 45.0f * delta);
    _player.subtractf_y(cosf(_player.get_angle()) * 45.0f * delta);
  }
  if (keysPressed.count('q') || keysPressed.count('Q')) {
    _player.addto_x(sinf(_player.get_angle() - (3.14159f / 2.0f)) * 45.0f *
                    delta);
    _player.addto_y(cosf(_player.get_angle() - (3.14159f / 2.0f)) * 45.0f *
                    delta);
  }
  if (keysPressed.count('e') || keysPressed.count('E')) {
    _player.addto_x(sinf(_player.get_angle() + (3.14159f / 2.0f)) * 45.0f *
                    delta);
    _player.addto_y(cosf(_player.get_angle() + (3.14159f / 2.0f)) * 45.0f *
                    delta);
  }
  if (keysPressed.count('x') || keysPressed.count('X')) {
    EngineState::GetInstance()->gameRunningf = false;
  }
}

#endif // OS Check end
