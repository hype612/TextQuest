#ifndef ASCIIVERSE_H
#define ASCIIVERSE_H

#include "EngineState.h"
#include "Entity.h"
#include "EntityManager.h"
#include "IInputHandler.h"
#include "IRenderer.h"
#include "MapManager.h"
#include "NCursesInputHandler.h"
#include "NCursesRenderer.h"
#include "RenderAssetManager.h"
#include "SceneManager.h"
#include "TextureLoader.h"
#include "TextureMapper.h"
#include "TextureRequestQueue.h"
#include "WindowsInputHandler.h"
#include "WindowsRenderer.h"
#include "player.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

class GameEngine {
private:
  // scene
  SceneManager _sceneManager;
  MapManager _mapManager;
  EntityManager _entityManager;

  Player _player;

  // rendering vars and consts
  IRenderer *_renderer;
  IInputHandler *_inputHandler;
  wchar_t *screen;
  int &_screenHeight = EngineState::GetInstance()->screenHeight;
  int &_screenWidth = EngineState::GetInstance()->screenWidth;
  TextureRequestQueue _texRequestQueue;
  RenderAssetManager _renderAssetManager;

  // for multithreading
  int _max_thread_num = -1;
  std::thread textureSetterT;

  // dont want anyone to call this
  GameEngine(GameEngine &g);
  // fallback map for testing
  bool initTestMap(); // for now initializes a static test map
public:
  GameEngine(int sc_width, int sc_height);
  void run_game();
  void SetScaledTexture(std::wstring &source, std::wstring &into,
                        float distance);
  void RayCastingProcess();
  void RenderScreen(int ceiling, int floor, int col, float distance_to_wall,
                    std::vector<std::tuple<std::wstring, int>> entities);
};

#endif // ASCIIVERSE_H
