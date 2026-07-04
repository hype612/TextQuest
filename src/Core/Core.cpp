#include "../Headers/Core.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <ncurses.h>
#include <notcurses/notcurses.h>
#include <string>

GameEngine::GameEngine(int sc_width, int sc_height)
    : _sceneManager(_entityManager, _mapManager),
      _player(_sceneManager.getPlayerRef()),
      _renderAssetManager(_entityManager, _mapManager, _texRequestQueue) {
  EngineState::GetInstance()->globalRenderAssetManager = &_renderAssetManager;
  EngineState::GetInstance()->globalSceneManager = &_sceneManager;
#if (defined(LINUX) || defined(__linux__))
  //_inputHandler = new NCursesInputHandler(_player);
  //_renderer = new NCursesRenderer();
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
  _inputHandler = new NotcursesInputHandler(_player, nc);
#endif
#if (defined(_WIN32) || defined(_WIN64))
  _renderer = new WindowsRenderer();
  _inputHandler = new WindowsInputHanlder(_player);
#endif
  _renderer->Init();
  _renderer->SetScreenSize(sc_width, sc_height);
  _renderAssetManager.setDistanceShading(true);
}

GameEngine::GameEngine()
    : _sceneManager(_entityManager, _mapManager),
      _player(_sceneManager.getPlayerRef()),
      _renderAssetManager(_entityManager, _mapManager, _texRequestQueue) {
  EngineState::GetInstance()->globalRenderAssetManager = &_renderAssetManager;
  EngineState::GetInstance()->globalSceneManager = &_sceneManager;
#if (defined(LINUX) || defined(__linux__))
  //_inputHandler = new NCursesInputHandler(_player);
  //_renderer = new NCursesRenderer();
  notcurses_options ncopts{getenv("TERM"),   NCLOGLEVEL_SILENT, 0, 0, 0, 0,
                           NCOPTION_CLI_MODE};
  std::shared_ptr<notcurses> nc(notcurses_core_init(&ncopts, stdout),
                                notcurses_stop);
  _renderer = new NotcursesRenderer(nc);
  _inputHandler = new NotcursesInputHandler(_player, nc);
#endif
#if (defined(_WIN32) || defined(_WIN64))
  _renderer = new WindowsRenderer();
  _inputHandler = new WindowsInputHanlder(_player);
#endif
  //_renderer->Init();
  // int row, col;
  // getmaxyx(stdscr, row, col);
  //_renderer->SetScreenSize(col, row);
}

void GameEngine::enableDistanceShading(bool enabled) {
  _renderAssetManager.setDistanceShading(enabled);
}
void GameEngine::setDistanceShadingThresholds(
    const std::vector<float> &thresholds) {
  _renderAssetManager.setDistanceShadingThresholds(thresholds);
}

bool GameEngine::initTestMap() {
  std::string map;
  map += "###############################";
  map += "#.............................#";
  map += "#..............###........#####";
  map += "#..............#............#.#";
  map += "#..............#..............#";
  map += "#..............#..............#";
  map += "#..............#..............#";
  map += "#..............#..............#";
  map += "#..............#..............#";
  map += "#..............#..............#";
  map += "#.............................#";
  map += "#..............#..............#";
  map += "###############################";

  int map_w = map.find("\n");
  int map_h = map.size() / map_w;

  _sceneManager.initializeNewMap(map, map_w, map_h);

  return true;
}

void GameEngine::run_game() {
  Logger::GetInstance()->log("main loop running", LogType::CORE,
                             LogLevel::INFO);
  Logger::forceFlush();
  EngineState::GetInstance()->gameRunningf = true;
  screen = new char[_screenWidth * _screenHeight];
  if (!_sceneManager.isMapAvailable())
    initTestMap();
  auto tp1 = std::chrono::system_clock::now();
  auto tp2 = std::chrono::system_clock::now();
  while (EngineState::GetInstance()->gameRunningf == true) {

    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed_time = tp2 - tp1;
    tp1 = tp2;
    float f_elapsed_time = elapsed_time.count();

    _inputHandler->ReceiveMovementInput(f_elapsed_time);
    _sceneManager.process();
    RayCastingProcess();

    _renderer->OverwriteBuffer(screen);
    _renderer->PrintDebugInfo(_player, f_elapsed_time);
    _renderer->PrintBuffer();
  }
}

void GameEngine::RayCastingProcess() {
  const float max_raylength = 50.f;
  for (int x = 0; x < _screenWidth; x++) {
    bool hitwall = false;

    // init for DDA
    WallSide side = WallSide::NOHIT;
    float rayLength = 0;
    _steps = 0;

    vec2f playerDir{std::sin(_player.getAngle()), std::cos(_player.getAngle())};
    float cameraX = 2.f * x / (float)_screenWidth - 1.f;
    vec2f planeV{
        std::cos(_player.getAngle()) * std::tan(_player.getFovInRad() / 2.f),
        -std::sin(_player.getAngle()) * std::tan(_player.getFovInRad() / 2.f)};

    vec2f rayDir{playerDir + planeV * cameraX};
    vec2f deltaDist{(rayDir.x == 0.f) ? 1e30f : std::abs(1.f / rayDir.x),
                    (rayDir.y == 0.f) ? 1e30f : std::abs(1.f / rayDir.y)};

    vec2i stepDir{(rayDir.x >= 0.f) ? 1 : -1, (rayDir.y >= 0.f) ? 1 : -1};
    vec2i mapPos{static_cast<int>(std::floor(_player.getX())),
                 static_cast<int>(std::floor(_player.getY()))};
    vec2f sideDist{(stepDir.x == 1) ? mapPos.x + 1.f - _player.getX()
                                    : _player.getX() - mapPos.x,
                   (stepDir.y == 1) ? mapPos.y + 1.f - _player.getY()
                                    : _player.getY() - mapPos.y};
    if (sideDist.x <= 0.0001f)
      sideDist.x = 1.f;
    if (sideDist.y <= 0.0001f)
      sideDist.y = 1.f;
    sideDist.x = sideDist.x * deltaDist.x;
    sideDist.y = sideDist.y * deltaDist.y;
    while (!hitwall && rayLength < max_raylength) {
      _steps++;
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

      if (mapPos.x < 0 || mapPos.y < 0 || mapPos.x >= _mapManager.mapWidth() ||
          mapPos.y >= _mapManager.mapHeight()) {
        sideDist.x = max_raylength;
        sideDist.y = max_raylength;
        break;
      }

      if (_sceneManager.isOccupied(mapPos.x, mapPos.y) == Tile::WALL) {
        hitwall = true;
        float dist = (side == WallSide::HORIZONTAL) ? sideDist.x - deltaDist.x
                                                    : sideDist.y - deltaDist.y;

        int height = (int)(_screenHeight / dist);
        int wallTop = (_screenHeight / 2) - (height / 2);
        float hitp = (side == WallSide::HORIZONTAL)
                         ? _player.getY() + dist * rayDir.y
                         : _player.getX() + dist * rayDir.x;
        hitp -= std::floorf(hitp);
        _texRequestQueue.push(
            {mapPos.x, mapPos.y, Tile::WALL, height, wallTop, hitp, dist});
      }
      if (_sceneManager.isOccupied(mapPos.x, mapPos.y) == Tile::ENTITY) {

        float dist = (side == WallSide::HORIZONTAL) ? sideDist.x - deltaDist.x
                                                    : sideDist.y - deltaDist.y;
        int height = (int)(_screenHeight / dist);
        int wallTop = (_screenHeight / 2) - (height / 2);
        float hitp = (side == WallSide::HORIZONTAL)
                         ? _player.getY() + dist * rayDir.y
                         : _player.getX() + dist * rayDir.x;
        hitp -= std::floorf(hitp);
        _texRequestQueue.push(
            {mapPos.x, mapPos.y, Tile::ENTITY, height, wallTop, hitp, dist});
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
    float hitpoint = (side == WallSide::HORIZONTAL)
                         ? _player.getY() + distance_to_wall * rayDir.y
                         : _player.getX() + distance_to_wall * rayDir.x;
    if (!hitwall) {
      side = WallSide::NOHIT;
      distance_to_wall = max_raylength;
      hitpoint = 0.f;
    }

    hitpoint -= std::floorf(hitpoint);
    if (distance_to_wall < 0.0001f)
      distance_to_wall = 0.0001f;

    int wallHeight = (int)(_screenHeight / distance_to_wall);
    int ceiling = (_screenHeight / 2) - (wallHeight / 2);
    int floor = ceiling + wallHeight;

    ceiling = std::max(0, ceiling);
    floor = std::min(_screenHeight - 1, floor);
    RenderScreen(ceiling, floor, x);
  }
}

void GameEngine::RenderScreen(int ceiling, int floor, int col) {
  wchar_t floorShade;
  int x = col;
  std::string toRender =
      _renderAssetManager.getNextCharColumn(floor - ceiling + 1);
  int toRenderIt = 0;
  int colHeight =
      std::min(toRender.size(), static_cast<size_t>(floor - ceiling + 1));
  for (int y = 0; y < _screenHeight; y++) {
    if (y < ceiling) {
      screen[y * _screenWidth + x] = ' ';
    } else if (y >= ceiling && y <= floor) {
      if (toRenderIt < colHeight) {
        screen[y * _screenWidth + x] = toRender[toRenderIt];
        toRenderIt++;
      } else {
        screen[y * _screenWidth + x] = ' ';
      }
    } else {
      float b = 1 - (((float)y - _screenHeight / 2.0f) /
                     ((float)_screenHeight / 2.0f));
      if (b < 0.25)
        floorShade = '#';
      else if (b < 0.5)
        floorShade = 'X';
      else if (b < 0.75)
        floorShade = '.';
      else if (b < 0.9)
        floorShade = '-';
      else
        floorShade = ' ';
      screen[y * _screenWidth + x] = floorShade;
    }
  }
}

GameEngine::~GameEngine() {
  delete _renderer;
  delete _inputHandler;
  delete[] screen;
}
