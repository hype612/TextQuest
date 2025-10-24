#include "../Headers/WindowsInputHandler.h"

#if (defined (_WIN32) || defined (_WIN64))

WindowsInputHandler::WindowsInputHandler(Player& player) : IInputHandler(player) {}

void WindowsInputHandler::Init() {
    // empty for now
}

void WindowsInputHandler::ReceiveMovementInput(float delta) {
	if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
		_player.subtractf_angle(1.2f * delta);
	}
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
		_player.addto_angle(1.2f * delta);
	}
	if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
		_player.addto_x(sinf(_player.get_angle()) * 5.0f * delta);
		_player.addto_y(cosf(_player.get_angle()) * 5.0f * delta);
	}
	if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
		_player.subtractf_x(sinf(_player.get_angle()) * 5.0f * delta);
		_player.subtractf_y(cosf(_player.get_angle()) * 5.0f * delta);
	}
	if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
		_player.addto_x(sinf(_player.get_angle() - (3.14159f / 2.0f)) * 5.0f * delta);
		_player.addto_y(cosf(_player.get_angle() - (3.14159f / 2.0f)) * 5.0f * delta);
	}
	if (GetAsyncKeyState((unsigned short)'E') & 0x8000) {
		_player.addto_x(sinf(_player.get_angle() + (3.14159f / 2.0f)) * 5.0f * delta);
		_player.addto_y(cosf(_player.get_angle() + (3.14159f / 2.0f)) * 5.0f * delta);
	}
}

#endif // OS Checker end
