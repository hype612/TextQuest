#include <iostream>
#include <Windows.h>
#include <chrono>
#include "Headers/player.h"
#include <vector>
#include <algorithm>
#include "Headers\AsciiVerse.h"
#include "Headers\TextureLoader.h"



int main() {
	GameEngine instance(120, 80, ".\\Resources\\Textures"); // must give res to screen
	instance.run_game();
	return 0;
}