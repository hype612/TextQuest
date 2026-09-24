#ifndef ASCIIVERSE_H
#define ASCIIVERSE_H

#include "IInputHandler.h"
#include "IRenderer.h"
#include "RenderAssetManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "WindowsInputHandler.h"
#include "WindowsRenderer.h"
#include <functional>
#include <memory>
#include <optional>
#include <vector>

class GameEngine {
public:
  GameEngine();
  void run_game();
  ~GameEngine();

  void setScene(const std::shared_ptr<SceneManager> &sc_man);

  void enableDistanceShading(bool enabled);
  void setDistanceShadingThresholds(const std::vector<float> &thresholds);
  vec2i screenSize();

  // TODO: Get rid of these ASAP
  //       only for testing purposes mid-refactor
  //       implement a real API for game programmers
  SceneManager &sceneMan();
  IInputHandler &inputHandler();
  UIManager &uiMan();

  void setOnSceneOver(std::function<void()> callback);
  void setPostSceneOver(std::function<void()> callback);

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
  void applyPendingScene();
  std::shared_ptr<SceneManager> _sceneManager; // active
  std::shared_ptr<SceneManager> _pendingScene; // applied at loop top
  std::function<void()> _sceneOverCb;
  std::function<void()> _postSceneOverCb;

  // rendering vars and consts
  bool _shadingEnabled = false;
  std::vector<float> _shadingThresholds;
  bool _engineRunning = false;
  bool _sceneOverFired = false;

  IRenderer *_renderer;
  IInputHandler *_inputHandler;
  char *screen;
  std::optional<RenderAssetManager> _renderAssetManager;
  UIManager *_uimanager;
  std::vector<float> _zBuffer;
};

#endif // ASCIIVERSE_H
