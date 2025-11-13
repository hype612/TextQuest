#include "../Headers/Core.h"

GameEngine::GameEngine(int sc_width = 240, int sc_height = 80) {
  // TODO: just decide in compile time
  // Obviously, it does not make sense to do this at runtime
  if (EngineState::GetInstance()->opSystem == RunningOS::WINDOWS) {
    //_renderer     = new WindowsRenderer();
    //_inputHandler = new WindowsInputHanlder(_player);
  } else if (EngineState::GetInstance()->opSystem == RunningOS::LINUX) {
    _renderer = new NCursesRenderer();
    _inputHandler = new NCursesInputHandler(_player);
  }

  _renderer->Init();
  _renderer->SetScreenSize(sc_width, sc_height);
  _max_thread_num = std::thread::hardware_concurrency();
}

bool GameEngine::initTestMap() {
  std::wstring map;
  map += L"###############################################################";
  map += L"#.............................................................#";
  map += L"#..............########################################....####";
  map += L"#..............#............#.................................#";
  map += L"#..............#............#.................................#";
  map += L"#..............#............#.................................#";
  map += L"#..............##############...........###...................#";
  map += L"#......#####...#.....................###...##.................#";
  map += L"#..............#....................#........#................#";
  map += L"#########......#....................#........#................#";
  map += L"#..............#.....................#......#.................#";
  map += L"#..............#.....................#......#.................#";
  map += L"#..............#.....................#......#.................#";
  map += L"#..............#.....................#......#.................#";
  map += L"#..............#.....................#......#.................#";
  map += L"#..............#.....................#......#.................#";
  map += L"#..............#.....................#......#.................#";
  map += L"#..............#...................##........##...............#";
  map += L"#......#####...#..................##..........##..............#";
  map += L"#..............#..................#....#..#....#..............#";
  map += L"#########......#...................####....####...............#";
  map += L"#..............#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#.............................................................#";
  map += L"#..............#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#......#####...#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#########......#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#..............#..............................................#";
  map += L"#.............................................................#";
  map += L"#..............#..............................................#";
  map += L"###############################################################";

  int map_w = map.find(L"\n");
  int map_h = map.size() / map_w;

  _sceneManager.initializeNewMap(map, map_w, map_h);

  return true;
}

void GameEngine::run_game() {
  // game loop
  if (!_sceneManager.isMapAvailable())
    initTestMap();

  auto tp1 = std::chrono::system_clock::now();
  auto tp2 = std::chrono::system_clock::now();

  tp2 = std::chrono::system_clock::now();
  std::chrono::duration<float> elapsed_time = tp2 - tp1;
  tp1 = tp2;
  float f_elapsed_time = elapsed_time.count();

  _inputHandler->ReceiveMovementInput(f_elapsed_time);
  _sceneManager.process();

  RayCastingProcess();

  screen[_screenHeight * _screenWidth - 1] = '\0';
  _renderer->OverwriteBuffer(screen);
  _renderer->PrintBuffer();
}

void GameEngine::RayCastingProcess() {
  bool hitwall;
  bool isedge;

  int currentObjX = -1;
  int currentObjY = -1;
  bool isCurrentObj = false;
  const int max_raylength = 8;

  for (int x = 0; x < _screenWidth; x++) {
    float ray_angle = (_player.get_angle() - _player.get_fov() / 2.0f) +
                      ((float)x / (float)_screenWidth) * _player.get_fov();
    float distance_to_wall = 0.0f;
    hitwall = false;
    isedge = false;

    float eye_x = sinf(ray_angle);
    float eye_y = cosf(ray_angle);

    int test_x;
    int test_y;
    while (!hitwall && distance_to_wall < max_raylength) {
      distance_to_wall += 0.1f;
      test_x = (int)(_player.get_x() + eye_x * distance_to_wall);
      test_y = (int)(_player.get_y() + eye_y * distance_to_wall);
      if (test_x < 0 || test_x >= _sceneManager.getMapWidth() || test_y < 0 ||
          test_y >= _sceneManager.getMapHeight()) {
        hitwall = true;
        distance_to_wall = max_raylength;
      } else {
        // ray is inbounds > test if is a wall block
        if (_sceneManager.isOccupied(test_x, test_y)) {
          hitwall = true;
          isCurrentObj = currentObjX == test_x && currentObjY == test_y;
          if (!isCurrentObj) {
            // textureSetterT = std::thread(TextureMapper::setCurrentTexture,
            // distance_to_wall, "repeat", &textures[textureFromMap.texture]);
            _texRequestQueue.push(std::tuple<int, int>(test_x, test_y));
            currentObjY = test_y;
            currentObjX = test_x;
          }
        }
      }
    }
    int ceiling = int((float)(_screenHeight / 2.0f) -
                      _screenHeight / ((float)distance_to_wall));
    int floor = _screenHeight - ceiling;

    RenderScreen(ceiling, floor, x, distance_to_wall, entitiesInWay);
  }
}

void GameEngine::RenderScreen(
    int ceiling, int floor, int col, float distance_to_wall,
    std::vector<std::tuple<std::wstring, int>> entities) {
  short shade;
  int x = col;
  std::wstring render_fg;
  if (textureSetterT.joinable()) {
    textureSetterT.join();
  }
  std::wstring render_bg =
      TextureMapper::GetCharColumnAtPosition(_screenHeight - ceiling);
  if (entities.size() > 0)
    render_fg = TextureMapper::GetCharColumnAtPosition(
        _screenHeight - ceiling, std::get<0>(entities.back()),
        std::get<1>(entities.back()));
  for (int y = 0; y < _screenHeight; y++) {
    if (y < ceiling) {
      screen[y * _screenWidth + x] = ' ';
    } else if (y > ceiling && y <= floor) {

      if (distance_to_wall > 8.f)
        screen[y * _screenWidth + x] = L' ';
      else
        screen[y * _screenWidth + x] = render_bg[y - ceiling];

      if (entities.size() > 0) {
        if (distance_to_wall > 8.f)
          screen[y * _screenWidth + x] = L' ';
        else
          screen[y * _screenWidth + x] = render_fg[y - ceiling];
      }
    } else {
      float b = 1.0f - (((float)y - _screenHeight / 2.0f) /
                        ((float)_screenHeight / 2.0f));
      if (b < 0.25)
        shade = '#';
      else if (b < 0.5)
        shade = 'X';
      else if (b < 0.75)
        shade = '.';
      else if (b < 0.9)
        shade = '-';
      else
        shade = ' ';
      screen[y * _screenWidth + x] = shade;
    }
  }
}
