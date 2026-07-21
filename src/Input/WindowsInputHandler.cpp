#include "../Headers/WindowsInputHandler.h"

#if (defined(_WIN32) || defined(_WIN64))

WindowsInputHandler::WindowsInputHandler(Player &player)
    : IInputHandler(player) {}

void WindowsInputHandler::Init() {
  // empty for now
}

void WindowsInputHandler::ReceiveMovementInput(float delta) {
  if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
    _player.subtractFromAngle(1.2f * delta);
  }
  if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
    _player.addToAngle(1.2f * delta);
  }
  if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
    _player.addToX(sinf(_player.getAngle()) * 5.0f * delta);
    _player.addToY(cosf(_player.getAngle()) * 5.0f * delta);
  }
  if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
    _player.subtractFromX(sinf(_player.getAngle()) * 5.0f * delta);
    _player.subtractFromY(cosf(_player.getAngle()) * 5.0f * delta);
  }
  if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
    _player.addToX(sinf(_player.getAngle() - (3.14159f / 2.0f)) * 5.0f * delta);
    _player.addToY(cosf(_player.getAngle() - (3.14159f / 2.0f)) * 5.0f * delta);
  }
  if (GetAsyncKeyState((unsigned short)'E') & 0x8000) {
    _player.addToX(sinf(_player.getAngle() + (3.14159f / 2.0f)) * 5.0f * delta);
    _player.addToY(cosf(_player.getAngle() + (3.14159f / 2.0f)) * 5.0f * delta);
  }
  if (GetAsyncKeyState((unsigned short)'X') & 0x8000) {
  }
}

#endif // OS Checker end
