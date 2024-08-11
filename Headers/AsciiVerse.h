#ifndef ASCIIVERSE_H
#define ASCIIVERSE_H

#include <iostream>
#include <Windows.h>
#include <chrono>
#include <vector>
#include <algorithm>
#include "Headers\player.h"


class GameEngine {
private:
	// screen consts
	int screen_width; // 120
	int screen_height; // 40

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

	// dont want anyone to call this
	GameEngine(GameEngine& g);

public:
	GameEngine(int sc_width, int sc_height);
	bool initMap(); // for now initializes a static test map
	void run_game();
};

#endif // !ASCIIVERSE_H
