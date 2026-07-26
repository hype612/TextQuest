#include "../Headers/Core.h"
#include "../Headers/Camera.h"
#include "../Headers/Logger.h"
#include "../Headers/Transform.h"
#include "NotcursesInputHandler.h"
#include "NotcursesRenderer.h"
#include "Vec2f.h"
#include "Vec2i.h"
#include "WallSide.h"
#include "WindowsInputHandler.h"
#include "WindowsRenderer.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <ncurses.h>
#include <notcurses/notcurses.h>
#include <string>

GameEngine::GameEngine()
    : _sceneManager(), _renderAssetManager(_sceneManager, _texRequestQueue) {

#if (defined(LINUX) || defined(__linux__))
  notcurses_options ncopts{getenv("TERM"),
                           NCLOGLEVEL_SILENT,
                           0,
                           0,
                           0,
                           0,
                           NCOPTION_NO_ALTERNATE_SCREEN |
                               NCOPTION_NO_CLEAR_BITMAPS};
  std::shared_ptr<notcurses> nc(notcurses_core_init(&ncopts, stdout),
                                notcurses_stop);
  _renderer = new NotcursesRenderer(nc);
  _inputHandler = new NotcursesInputHandler(nc);
  Logger::GetInstance()->log(
      "nc constructed at: " +
          std::to_string(reinterpret_cast<uintptr_t>(nc.get())),
      LogType::CORE, LogLevel::INFO);
#endif
#if (defined(_WIN32) || defined(_WIN64))
  _renderer = new WindowsRenderer();
  _inputHandler = new WindowsInputHanlder(_player);
#endif
}

void GameEngine::enableDistanceShading(bool enabled) {
  _renderAssetManager.setDistanceShading(enabled);
}
void GameEngine::setDistanceShadingThresholds(
    const std::vector<float> &thresholds) {
  _renderAssetManager.setDistanceShadingThresholds(thresholds);
}

SceneManager &GameEngine::sceneMan() { return _sceneManager; }
IInputHandler &GameEngine::inputHandler() { return *_inputHandler; }

void GameEngine::run_game() {
  _sceneRunning = true;
  screen = new char[_renderer->screenWidth() * _renderer->screenHeight()];
  if (!_sceneManager.isMapAvailable()) {
    Logger::GetInstance()->log("Map was never uploaded. Shutting down GameLoop",
                               LogType::CORE, LogLevel::ERROR);
    return;
  }
  auto tp1 = std::chrono::system_clock::now();
  auto tp2 = std::chrono::system_clock::now();
  while (_sceneRunning) {
    const Camera *cam = _sceneManager.cameraPtr();
    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed_time = tp2 - tp1;
    tp1 = tp2;
    float f_elapsed_time = elapsed_time.count();

    _inputHandler->ReceiveInput();
    _sceneManager.process(f_elapsed_time);
    RayCastingProcess(cam);

    _renderer->OverwriteBuffer(screen);
    std::vector<std::string> dbgNfo = {
        "C.x: " + std::to_string(cam->follow().position.x) +
            " C.y: " + std::to_string(cam->follow().position.y) +
            " angle: " + std::to_string(cam->follow().angle),
        "fps: " + std::to_string(1.f / f_elapsed_time)};
    _renderer->PrintDebugInfo(dbgNfo);
    _renderer->PrintBuffer();
  }
}

void GameEngine::RayCastingProcess(const Camera *cam) {
  const float max_raylength = 50.f;
  int screenHeight = _renderer->screenHeight();
  int screenWidth = _renderer->screenWidth();
  vec2f playerDir{std::sin(cam->follow().angle), std::cos(cam->follow().angle)};
  vec2f planeV{std::cos(cam->follow().angle) * std::tan(cam->fov() / 2.f),
               -std::sin(cam->follow().angle) * std::tan(cam->fov() / 2.f)};
  for (int x = 0; x < screenWidth; x++) {
    bool hitwall = false;

    // init for DDA
    WallSide side = WallSide::NOHIT;
    float rayLength = 0;

    float cameraX = 2.f * x / (float)screenWidth - 1.f;
    vec2f rayDir{playerDir + planeV * cameraX};
    vec2f deltaDist{(rayDir.x == 0.f) ? 1e30f : std::abs(1.f / rayDir.x),
                    (rayDir.y == 0.f) ? 1e30f : std::abs(1.f / rayDir.y)};

    vec2i stepDir{(rayDir.x >= 0.f) ? 1 : -1, (rayDir.y >= 0.f) ? 1 : -1};
    vec2i mapPos{static_cast<int>(std::floor(cam->follow().position.x)),
                 static_cast<int>(std::floor(cam->follow().position.y))};
    vec2f sideDist{(stepDir.x == 1) ? mapPos.x + 1.f - cam->follow().position.x
                                    : cam->follow().position.x - mapPos.x,
                   (stepDir.y == 1) ? mapPos.y + 1.f - cam->follow().position.y
                                    : cam->follow().position.y - mapPos.y};
    if (sideDist.x <= 0.0001f)
      sideDist.x = 1.f;
    if (sideDist.y <= 0.0001f)
      sideDist.y = 1.f;
    sideDist.x = sideDist.x * deltaDist.x;
    sideDist.y = sideDist.y * deltaDist.y;
    while (!hitwall && rayLength < max_raylength) {
      if (sideDist.x < sideDist.y) {
        sideDist.x += deltaDist.x;
        mapPos.x += stepDir.x;
        rayLength = sideDist.x;
        side = WallSide::HORIZONTAL;
      } else {
        sideDist.y += deltaDist.y;
        mapPos.y += stepDir.y;
        rayLength = sideDist.y;
        side = WallSide::VERTICAL;
      }

      if (_sceneManager.isOutOfBounds(mapPos.x, mapPos.y)) {
        sideDist.x = max_raylength;
        sideDist.y = max_raylength;
        break;
      }

      if (_sceneManager.isWall(mapPos.x, mapPos.y)) {
        hitwall = true;
      }
      std::string sideStr;
      switch (side) {
      case WallSide::NOHIT:
        sideStr = "NOHIT";
        break;
      case WallSide::HORIZONTAL:
        sideStr = "HORIZONTAL";
        break;
      case WallSide::VERTICAL:
        sideStr = "VERTICAL";
        break;
      }
    }
    float distance_to_wall = (side == WallSide::HORIZONTAL)
                                 ? sideDist.x - deltaDist.x
                                 : sideDist.y - deltaDist.y;
    float hitpoint =
        (side == WallSide::HORIZONTAL)
            ? cam->follow().position.y + distance_to_wall * rayDir.y
            : cam->follow().position.x + distance_to_wall * rayDir.x;

    hitpoint -= std::floorf(hitpoint);
    if (distance_to_wall < 0.0001f)
      distance_to_wall = 0.0001f;

    int wallHeight = (int)(screenHeight / distance_to_wall);

    int ceiling = (screenHeight / 2) - (wallHeight / 2);
    int floor = ceiling + wallHeight;
    ceiling = std::max(0, ceiling);
    floor = std::min(screenHeight - 1, floor);
    if (!hitwall) {
      side = WallSide::NOHIT;
      distance_to_wall = max_raylength;
      hitpoint = 0.f;
      ceiling = screenHeight / 2;
      floor = screenHeight / 2;
    } else {
      int wallTop = (screenHeight / 2) - (wallHeight / 2);
      int visibleTop = std::max(0, -wallTop);
      int visibleBot = std::min(screenHeight - wallTop, (int)wallHeight);
      _texRequestQueue.push({mapPos.x, mapPos.y, wallHeight, visibleTop,
                             visibleBot, hitpoint, distance_to_wall});
    }
    RenderScreen(ceiling, floor, x, screenWidth, screenHeight);
  }
}

void GameEngine::RenderScreen(int ceiling, int floor, int col, int screenWidth,
                              int screenHeight) {
  wchar_t floorShade;
  int x = col;
  std::string toRender =
      _renderAssetManager.getNextCharColumn(floor - ceiling + 1);
  int toRenderIt = 0;

  int colHeight =
      std::min(toRender.size(), static_cast<size_t>(floor - ceiling + 1));
  for (int y = 0; y < screenHeight; y++) {
    if (y < ceiling) {
      screen[y * screenWidth + x] = ' ';
    } else if (y >= ceiling && y <= floor) {
      if (toRenderIt < colHeight) {
        screen[y * screenWidth + x] = toRender[toRenderIt];
        toRenderIt++;
      } else {
        screen[y * screenWidth + x] = ' ';
      }
    } else {
      float b =
          1 - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
      if (b < 0.25)
        floorShade = '#';
      else if (b < 0.5)
        floorShade = 'x';
      else if (b < 0.75)
        floorShade = '.';
      else if (b < 0.9)
        floorShade = '-';
      else
        floorShade = ' ';
      screen[y * screenWidth + x] = floorShade;
    }
  }
}

GameEngine::~GameEngine() {
  delete _renderer;
  delete _inputHandler;
  delete[] screen;
}
