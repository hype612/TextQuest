#ifndef ASCIIVERSE_H
#define ASCIIVERSE_H

#include <iostream>
#include <Windows.h>
#include <chrono>
#include <vector>
#include <algorithm>
#include "Headers\player.h"
#include "Headers\TextureLoader.h"
#include "Headers\TextureMapper.h"

class GameEngine {
private:
	// screen consts
	int screen_width; 
	int screen_height; 
	struct tile {
		int position = -1; // since the screen is a 2d array
		std::vector<std::pair<float, float>> edges;
	};

	// map consts
	int map_height = 16;
	int map_width = 16;
	float max_raylength = 16.0f;
	std::wstring map;

	Player player;

	// rendering vars and consts
	wchar_t* screen;
	HANDLE hConsole;
	DWORD dwBytesWritten;
	std::unordered_map<wchar_t, std::wstring> textures;

	// dont want anyone to call this
	GameEngine(GameEngine& g);

public:
	GameEngine(int sc_width, int sc_height, std::string textures_path);
	bool initMap(); // for now initializes a static test map
	void run_game();
};

#endif // !ASCIIVERSE_H
