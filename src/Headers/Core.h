#ifndef ASCIIVERSE_H
#define ASCIIVERSE_H

#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <thread>
#include "player.h"
#include "TextureLoader.h"
#include "TextureMapper.h"
#include "TextureTile.h"
#include "Entity.h"
#include "IRenderer.h"
#include "WindowsRenderer.h"
#include "NCursesRenderer.h"
#include "IInputHandler.h"
#include "NCursesInputHandler.h"
#include "WindowsInputHandler.h"
#include "EngineState.h"

class GameEngine {
private:
	struct tile {
		int position = -1; // since the screen is a 2d array
		std::vector<std::pair<float, float>> edges;
	};

	// map consts
	int map_height = 39;
	int map_width = 63;
	float max_raylength = 16.0f;
	std::wstring map;
	std::vector<TextureTile> texMap;

	Player _player;

	std::vector<Entity> _entityContainer;
	IRenderer* _renderer;
	IInputHandler* _inputHandler;
	wchar_t* screen;
	int& _screenHeight = EngineState::GetInstance()->screenHeight;
	int& _screenWidth  = EngineState::GetInstance()->screenWidth;
	// rendering vars and consts
	std::unordered_map<wchar_t, std::wstring> textures;
	int _max_thread_num = -1;
	std::thread textureSetterT;

	// dont want anyone to call this
	GameEngine(GameEngine& g);

public:
	GameEngine(int sc_width, int sc_height, std::string textures_path);
	bool initMap(); // for now initializes a static test map
	void run_game();
	void SetScaledTexture(std::wstring& source, std::wstring& into, float distance);
	void RayCastingProcess();
	void RenderScreen(int ceiling, int floor, int col, float distance_to_wall, std::vector<std::tuple<std::wstring, int>> entities);
};

#endif // ASCIIVERSE_H
