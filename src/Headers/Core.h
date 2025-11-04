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
#include "SceneManager.h"
#include "MapManager.h"
#include "EntityManager.h"


class GameEngine {
private:
  // scene
  SceneManager _sceneManager; 

	// rendering vars and consts
	IRenderer* _renderer;
	IInputHandler* _inputHandler;
	wchar_t* screen;
	int& _screenHeight = EngineState::GetInstance()->screenHeight;
	int& _screenWidth  = EngineState::GetInstance()->screenWidth;

	// dont want anyone to call this
	GameEngine(GameEngine& g);

  // for multithreading
	int _max_thread_num = -1;
	std::thread textureSetterT;
public:
	GameEngine(int sc_width, int sc_height, std::string textures_path);
	bool initMap(); // for now initializes a static test map
	void run_game();
	void SetScaledTexture(std::wstring& source, std::wstring& into, float distance);
	void RayCastingProcess();
	void RenderScreen(int ceiling, int floor, int col, float distance_to_wall, std::vector<std::tuple<std::wstring, int>> entities);
};

#endif // ASCIIVERSE_H
