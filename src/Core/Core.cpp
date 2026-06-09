#include "../Headers/Core.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <ncurses.h>
#include <random>
#include <ratio>
#include <string>

GameEngine::GameEngine(int sc_width, int sc_height)
    : _sceneManager(_entityManager, _mapManager),
      _player(_sceneManager.getPlayerRef()),
      _renderAssetManager(_entityManager, _mapManager, _texRequestQueue) {
  EngineState::GetInstance()->globalRenderAssetManager = &_renderAssetManager;
  EngineState::GetInstance()->globalSceneManager = &_sceneManager;
  _max_thread_num = std::thread::hardware_concurrency();
#if (defined(LINUX) || defined(__linux__))
  _renderer = new NCursesRenderer();
  _inputHandler = new NCursesInputHandler(_player);
#endif
#if (defined(_WIN32) || defined(_WIN64))
  _renderer = new WindowsRenderer();
  _inputHandler = new WindowsInputHanlder(_player);
#endif
  _renderer->Init();
  _renderer->SetScreenSize(sc_width, sc_height);
}

GameEngine::GameEngine()
    : _sceneManager(_entityManager, _mapManager),
      _player(_sceneManager.getPlayerRef()),
      _renderAssetManager(_entityManager, _mapManager, _texRequestQueue) {
  EngineState::GetInstance()->globalRenderAssetManager = &_renderAssetManager;
  EngineState::GetInstance()->globalSceneManager = &_sceneManager;
  //_max_thread_num = std::thread::hardware_concurrency();
#if (defined(LINUX) || defined(__linux__))
  _renderer = new NCursesRenderer();
  _inputHandler = new NCursesInputHandler(_player);
#endif
#if (defined(_WIN32) || defined(_WIN64))
  _renderer = new WindowsRenderer();
  _inputHandler = new WindowsInputHanlder(_player);
#endif
  _renderer->Init();
  int row, col;
  getmaxyx(stdscr, row, col);
  _renderer->SetScreenSize(col, row);
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
  EngineState::GetInstance()->gameRunningf = true;
  screen = new char[_screenWidth * _screenHeight];
  //_textureSetterT =
  //    std::thread(&RenderAssetManager::TexturePreparator,
  //    &_renderAssetManager);
  if (!_sceneManager.isMapAvailable())
    initTestMap();
  auto tp1 = std::chrono::system_clock::now();
  auto tp2 = std::chrono::system_clock::now();
  while (EngineState::GetInstance()->gameRunningf == true) {
    /*
    if (_textureSetterT.joinable() &&
        !EngineState::GetInstance()->gameRunningf) {
      _textureSetterT.join();
    }*/

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
  const float max_raylength = 8.f;

  for (int x = 0; x < _screenWidth; x++) {
    // removed for trying single-thread performance
    //_texRequestQueue.setRayCompleted(false);
    //_texRequestQueue.setTexturesReady(false);
    float ray_angle = (_player.get_angle() - _player.get_fov_rad() / 2.0f) +
                      ((float)x / (float)_screenWidth) * _player.get_fov_rad();
    // float distance_to_wall = 0.0f;
    bool hitwall = false;

    // init for DDA
    float rayDirX = std::sinf(ray_angle);
    float rayDirY = std::cosf(ray_angle);
    float deltaDistX = (rayDirX == 0.f) ? 1e30f : std::fabs(1.f / rayDirX);
    float deltaDistY = (rayDirY == 0.f) ? 1e30f : std::fabs(1.f / rayDirY);
    int stepX = (rayDirX >= 0.f) ? 1 : -1;
    int stepY = (rayDirY >= 0.f) ? 1 : -1;
    float sideDistX = (stepX == 1)
                          ? std::floorf(_player.get_x()) + 1.f - _player.get_x()
                          : _player.get_x() - std::floorf(_player.get_x());
    if (sideDistX < 0.0001f)
      sideDistX = 1.f;
    sideDistX = sideDistX * deltaDistX;
    float sideDistY = (stepY == 1)
                          ? std::floorf(_player.get_y()) + 1.f - _player.get_y()
                          : _player.get_y() - std::floorf(_player.get_y());
    if (sideDistY < 0.0001f)
      sideDistY = 1.f;
    sideDistY = sideDistY * deltaDistY;

    int mapX = std::floorf(_player.get_x());
    int mapY = std::floorf(_player.get_y());

    WallSide side = WallSide::NOHIT;
    float rayLength = 0;
    _steps = 0;
    while (!hitwall && rayLength < max_raylength) {
      _steps++;
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = WallSide::HORIZONTAL;
      } else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = WallSide::VERTICAL;
      }

      if (mapX < 0 || mapY < 0 || mapX >= _mapManager.mapWidth() ||
          mapY >= _mapManager.mapHeight()) {
        sideDistX = max_raylength;
        sideDistY = max_raylength;
        break;
      }

      if (_sceneManager.isOccupied(mapX, mapY) == Tile::WALL) {
        hitwall = true;
        float dist = (side == WallSide::HORIZONTAL) ? sideDistX - deltaDistX
                                                    : sideDistY - deltaDistY;

        int height = (int)(_screenHeight / dist);
        float hitp = (side == WallSide::HORIZONTAL)
                         ? _player.get_y() + dist * rayDirY
                         : _player.get_x() + dist * rayDirX;
        hitp -= std::floorf(hitp);
        _texRequestQueue.push({mapX, mapY, Tile::WALL, height, hitp, dist});
      }
      if (_sceneManager.isOccupied(mapX, mapY) == Tile::ENTITY) {

        float dist = (side == WallSide::HORIZONTAL) ? sideDistX - deltaDistX
                                                    : sideDistY - deltaDistY;
        int height =
            int((float)(_screenHeight / 2.f) - _screenHeight / ((float)dist));
        float hitp = (side == WallSide::HORIZONTAL)
                         ? _player.get_y() + dist * rayDirY
                         : _player.get_x() + dist * rayDirX;
        hitp -= std::floorf(hitp);
        _texRequestQueue.push({mapX, mapY, Tile::ENTITY, height, hitp, dist});
      }
      rayLength = (side == WallSide::HORIZONTAL) ? sideDistX : sideDistY;
    }
    // Removed for trying single-threaded performance.
    //_texRequestQueue.setRayCompleted(true);
    //_texRequestQueue.waitForTextures();
    float distance_to_wall = (side == WallSide::HORIZONTAL)
                                 ? sideDistX - deltaDistX
                                 : sideDistY - deltaDistY;
    float hitpoint = (side == WallSide::HORIZONTAL)
                         ? _player.get_y() + distance_to_wall * rayDirY
                         : _player.get_x() + distance_to_wall * rayDirX;
    if (side == WallSide::NOHIT) {
      distance_to_wall = max_raylength;
      hitpoint = 0.f;
    }

    hitpoint -= std::floorf(hitpoint);
    // int ceiling = int((float)(_screenHeight / 2.f) -
    //                   _screenHeight / ((float)distance_to_wall));
    // int floor = _screenHeight - ceiling;
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
      float b = 1.0f - (((float)y - _screenHeight / 2.0f) /
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
