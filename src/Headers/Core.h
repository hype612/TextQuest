#ifndef ASCIIVERSE_H
#define ASCIIVERSE_H

#include "IInputHandler.h"
#include "IRenderer.h"
#include "RenderAssetManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "WindowsInputHandler.h"
#include "WindowsRenderer.h"
#include <vector>

class GameEngine {
public:
  GameEngine();
  void run_game();
  ~GameEngine();

  void enableDistanceShading(bool enabled);
  void setDistanceShadingThresholds(const std::vector<float> &thresholds);
  vec2i screenSize();

  // TODO: Get rid of these ASAP
  // only for testing purposes mid-refactor
  // implement a real API for game programmers
  SceneManager &sceneMan();
  IInputHandler &inputHandler();
  UIManager &uiMan();

  GameEngine(const GameEngine &g) = delete;
  GameEngine &operator=(const GameEngine &) = delete;
  GameEngine(GameEngine &) = delete;
  GameEngine &operator=(GameEngine &&) = delete;

private:
  void RayCastingProcess(const Camera *cam);
  void EntityProjectionProcess(const Camera *cam);
  void RenderCol(int ceiling, int floor, int col, int screenWidth,
                 int screenHeight, const std::string &toRender);
  // scene
  SceneManager _sceneManager;
  bool _sceneRunning = false;

  // rendering vars and consts
  IRenderer *_renderer;
  IInputHandler *_inputHandler;
  char *screen;
  RenderAssetManager _renderAssetManager;
  UIManager *_uimanager;
  std::vector<float> _zBuffer;
};

#endif // ASCIIVERSE_H
