#include "../Headers/NCursesInputHandler.h"


#if (defined (LINUX) || defined (__linux__))

NCursesInputHandler::NCursesInputHandler(Player& player) : IInputHandler(player) {}

void NCursesInputHandler::Init() {
    // check if ncurses has been inited
    // it should be, since renderer is inited first
}

void NCursesInputHandler::ReceiveMovementInput(float delta) {
    int keyPressed = getch();
    if (keyPressed == 'a' || keyPressed == 'A') {
		_player.subtractf_angle(1.2f * delta);
	}
	if (keyPressed == 'd' || keyPressed == 'D') {
		_player.addto_angle(1.2f * delta);
	}
	if (keyPressed == 'w' || keyPressed == 'W') {
		_player.addto_x(sinf(_player.get_angle()) * 5.0f * delta);
		_player.addto_y(cosf(_player.get_angle()) * 5.0f * delta);
	}
	if (keyPressed == 's' || keyPressed == 'S') {
		_player.subtractf_x(sinf(_player.get_angle()) * 5.0f * delta);
		_player.subtractf_y(cosf(_player.get_angle()) * 5.0f * delta);
	}
	if (keyPressed == 'q' || keyPressed == 'Q') {
		_player.addto_x(sinf(_player.get_angle() - (3.14159f / 2.0f)) * 5.0f * delta);
		_player.addto_y(cosf(_player.get_angle() - (3.14159f / 2.0f)) * 5.0f * delta);
	}
	if (keyPressed == 'e' || keyPressed == 'E') {
		_player.addto_x(sinf(_player.get_angle() + (3.14159f / 2.0f)) * 5.0f * delta);
		_player.addto_y(cosf(_player.get_angle() + (3.14159f / 2.0f)) * 5.0f * delta);
	}
}

#endif // OS Check end
