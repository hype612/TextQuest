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
public:
  GameEngine(int sc_width, int sc_height);
  GameEngine();
  void run_game();
  void SetScaledTexture(std::wstring &source, std::wstring &into,
                        float distance);
  void RayCastingProcess();
  void RenderScreen(int ceiling, int floor, int col);
  ~GameEngine();

  GameEngine(const GameEngine &g) = delete;
  GameEngine &operator=(const GameEngine &) = delete;
  GameEngine(GameEngine &) = delete;
  GameEngine &operator=(GameEngine &&) = delete;

private:
  // scene
  MapManager _mapManager;
  EntityManager _entityManager;
  SceneManager _sceneManager;

  Player &_player;

  // rendering vars and consts
  IRenderer *_renderer;
  IInputHandler *_inputHandler;
  wchar_t *screen;
  int &_screenHeight = EngineState::GetInstance()->screenHeight;
  int &_screenWidth = EngineState::GetInstance()->screenWidth;
  TextureRequestQueue _texRequestQueue;
  RenderAssetManager _renderAssetManager;
  std::thread _textureSetterT;

  // for multithreading
  int _max_thread_num = -1;
  std::thread textureSetterT;

  // dont want anyone to call this
  // fallback map for testing
  bool initTestMap(); // for now initializes a static test map
};

#endif // ASCIIVERSE_H
