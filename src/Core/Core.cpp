#include "../Headers/Core.h"
#include <algorithm>
#include <ncurses.h>
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
  int row, col;
  getmaxyx(stdscr, row, col);
  _renderer->SetScreenSize(col, row);
}

bool GameEngine::initTestMap() {
  std::string map;
  map += "###############################################################";
  map += "#.............................................................#";
  map += "#..............########################################....####";
  map += "#..............#............#.................................#";
  map += "#..............#............#.................................#";
  map += "#..............#............#.................................#";
  map += "#..............##############...........###...................#";
  map += "#......#####...#.....................###...##.................#";
  map += "#..............#....................#........#................#";
  map += "#########......#....................#........#................#";
  map += "#..............#.....................#......#.................#";
  map += "#..............#.....................#......#.................#";
  map += "#..............#.....................#......#.................#";
  map += "#..............#.....................#......#.................#";
  map += "#..............#.....................#......#.................#";
  map += "#..............#.....................#......#.................#";
  map += "#..............#.....................#......#.................#";
  map += "#..............#...................##........##...............#";
  map += "#......#####...#..................##..........##..............#";
  map += "#..............#..................#....#..#....#..............#";
  map += "#########......#...................####....####...............#";
  map += "#..............#..............................................#";
  map += "#..............#..............................................#";
  map += "#.............................................................#";
  map += "#..............#..............................................#";
  map += "#..............#..............................................#";
  map += "#..............#..............................................#";
  map += "#..............#..............................................#";
  map += "#......#####...#..............................................#";
  map += "#..............#..............................................#";
  map += "#########......#..............................................#";
  map += "#..............#..............................................#";
  map += "#..............#..............................................#";
  map += "#..............#..............................................#";
  map += "#..............#..............................................#";
  map += "#..............#..............................................#";
  map += "#..............#..............................................#";
  map += "#.............................................................#";
  map += "#..............#..............................................#";
  map += "###############################################################";

  int map_w = map.find("\n");
  int map_h = map.size() / map_w;

  _sceneManager.initializeNewMap(map, map_w, map_h);

  return true;
}

void GameEngine::run_game() {
  EngineState::GetInstance()->gameRunningf = true;
  screen = new char[_screenWidth * _screenHeight];
  _textureSetterT =
      std::thread(&RenderAssetManager::TexturePreparator, &_renderAssetManager);
  if (!_sceneManager.isMapAvailable())
    initTestMap();
  auto tp1 = std::chrono::system_clock::now();
  auto tp2 = std::chrono::system_clock::now();
  while (EngineState::GetInstance()->gameRunningf == true) {
    if (_textureSetterT.joinable() &&
        !EngineState::GetInstance()->gameRunningf) {
      _textureSetterT.join();
    }
    tp2 = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed_time = tp2 - tp1;
    tp1 = tp2;
    float f_elapsed_time = elapsed_time.count();

    _inputHandler->ReceiveMovementInput(f_elapsed_time);
    _sceneManager.process();

    RayCastingProcess();

    // screen[_screenHeight * _screenWidth - 1] = '\0';
    _renderer->OverwriteBuffer(screen);
    _renderer->PrintDebugInfo(_player, f_elapsed_time);
    _renderer->PrintBuffer();
  }
}

void GameEngine::RayCastingProcess() {
  bool hitwall;

  int currentObjX = -1;
  int currentObjY = -1;
  bool isCurrentObj = false;
  const int max_raylength = 8;

  static float prev_ceiling;
  bool firstCeilingCalculation = true;

  for (int x = 0; x < _screenWidth; x++) {
    _texRequestQueue.setRayCompleted(false);
    _texRequestQueue.setTexturesReady(false);
    float ray_angle = (_player.get_angle() - _player.get_fov() / 2.0f) +
                      ((float)x / (float)_screenWidth) * _player.get_fov();
    float distance_to_wall = 0.0f;
    hitwall = false;

    float eye_x = sinf(ray_angle);
    float eye_y = cosf(ray_angle);

    float f_test_x;
    float f_test_y;
    int test_x;
    int test_y;
    while (!hitwall && distance_to_wall < max_raylength) {
      distance_to_wall += .1f;

      f_test_x = (_player.get_x() + eye_x * distance_to_wall);
      f_test_y = (_player.get_y() + eye_y * distance_to_wall);

      test_x = (int)f_test_x;
      test_y = (int)f_test_y;
      if (test_x < 0 || test_x >= _sceneManager.getMapWidth() || test_y < 0 ||
          test_y >= _sceneManager.getMapHeight()) {
        hitwall = true;
        distance_to_wall = max_raylength;
      } else {
        // ray is inbounds > test if is a wall block
        if (_sceneManager.isOccupied(test_x, test_y) == Tile::WALL &&
            (currentObjX != test_x || currentObjY != test_y)) {
          hitwall = true;
          isCurrentObj = currentObjX == test_x && currentObjY == test_y;
          if (!isCurrentObj) {
            _texRequestQueue.push(std::tuple<int, int, Tile, float>(
                test_x, test_y, Tile::WALL, distance_to_wall));
            // currentObjY = test_y;
            // currentObjX = test_x;

            // calculate hitpoint
          }
        }
        if (_sceneManager.isOccupied(test_x, test_y) == Tile::ENTITY &&
            (currentObjX != test_x || currentObjY != test_y)) {
          isCurrentObj = currentObjX == test_x && currentObjY == test_y;
          if (!isCurrentObj) {
            _texRequestQueue.push(std::tuple<int, int, Tile, float>(
                test_x, test_y, Tile::ENTITY, distance_to_wall));
            currentObjY = test_y;
            currentObjX = test_x;
          }
        }
      }
    }
    /*int ceiling = int((float)(_screenHeight / 2.0f) -
                      _screenHeight / ((float)distance_to_wall));
    int floor = _screenHeight - ceiling;*/

    float target_ceiling =
        (_screenHeight / 2.0f) - (_screenHeight / distance_to_wall);
    if (firstCeilingCalculation) {
      prev_ceiling = target_ceiling;
      firstCeilingCalculation = false;
    }
    float smooth_factor = 0.01f; // 0 < factor <= 1
    float ceiling_f =
        prev_ceiling + (target_ceiling - prev_ceiling) * smooth_factor;
    int ceiling = static_cast<int>(ceiling_f + 0.5f);
    prev_ceiling = ceiling_f;

    int floor = _screenHeight - ceiling;

    _texRequestQueue.setRayCompleted(true);
    _texRequestQueue.waitForTextures();
    RenderScreen(ceiling, floor, x);
  }
}

void GameEngine::RenderScreen(int ceiling, int floor, int col) {
  wchar_t floorShade;
  int x = col;
  Logger::GetInstance()->log("floor: " + std::to_string(floor) +
                                 " ceiling: " + std::to_string(ceiling),
                             LogType::RENDER, LogLevel::INFO);
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
