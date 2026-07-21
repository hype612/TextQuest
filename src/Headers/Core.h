#ifndef ASCIIVERSE_H
#define ASCIIVERSE_H

#include "EntityManager.h"
#include "IInputHandler.h"
#include "IRenderer.h"
#include "MapManager.h"
#include "RenderAssetManager.h"
#include "SceneManager.h"
#include "TextureRequestQueue.h"
#include "WindowsInputHandler.h"
#include "WindowsRenderer.h"
#include <vector>

class GameEngine {
public:
  GameEngine();
  void run_game();
  void RayCastingProcess();
  void RenderScreen(int ceiling, int floor, int col, int screenWidth,
                    int screenHeight);
  ~GameEngine();

  void enableDistanceShading(bool enabled);
  void setDistanceShadingThresholds(const std::vector<float> &thresholds);
  // TODO: Get rid of this ASAP
  // only for testing purposes mid-refactor
  SceneManager &sceneMan();

  GameEngine(const GameEngine &g) = delete;
  GameEngine &operator=(const GameEngine &) = delete;
  GameEngine(GameEngine &) = delete;
  GameEngine &operator=(GameEngine &&) = delete;

private:
  // scene
  MapManager _mapManager;
  EntityManager _entityManager;
  SceneManager _sceneManager;
  const Camera *_camera;
  bool _sceneRunning = false;

  // rendering vars and consts
  IRenderer *_renderer;
  IInputHandler *_inputHandler;
  char *screen;
  TextureRequestQueue _texRequestQueue;
  RenderAssetManager _renderAssetManager;
};

#endif // ASCIIVERSE_H
