#include <iostream>
#include <Windows.h>
#include <chrono>
#include "Headers/player.h"
#include <vector>
#include <algorithm>
#include "Headers\AsciiVerse.h"
#include "Headers\TextureLoader.h"



int main() {
	GameEngine instance(240, 80); // must give res to screen
	//instance.run_game();
	std::unordered_map<wchar_t, std::wstring> Textures = Texture_loader::load_textures(".\\Resources\\Textures");
	
	std::cout << "BRICKWALL:\n";
	std::wcout << Textures[L'#'] << std::endl;
	std::cout << "SLIME:\n";
	std::wcout << Textures[L'S'] << std::endl;
	return 0;
}