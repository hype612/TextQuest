#include "../Headers/NCursesRenderer.h"
#include <ncurses.h>

#if (defined(LINUX) || defined(__linux__))

NCursesRenderer::NCursesRenderer() { Init(); }

void NCursesRenderer::Init() {
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(0);
  _screenHeight = EngineState::GetInstance()->screenHeight;
  _screenWidth = EngineState::GetInstance()->screenWidth;
  _screenBuffer = nullptr;
}

void NCursesRenderer::OverwriteBuffer(char *newBuffer) {
  if (_screenBuffer == nullptr) {
    std::cerr << "ERROR: no screenbuffer has been initialized yet. Please call "
                 "SetScreenSize before calling OverWriteBuffer"
              << std::endl;
    return;
  }
  if (newBuffer == nullptr) {
    std::cerr << "OverWriteBuffer: function was called with nullptr"
              << std::endl;
    return;
  }
  _screenBuffer = newBuffer;

  if (!_screenBuffer)
    return;

  for (int y = 0; y < _screenHeight; y++) {
    move(y, 0);
    std::string line(_screenBuffer + y * _screenWidth, _screenWidth);
    mvaddnstr(y, 0, line.c_str(), _screenWidth);
  }
}

void NCursesRenderer::PrintBuffer() { refresh(); }

void NCursesRenderer::PrintDebugInfo(const Player &player, float delta) {
  int debugY = 0;
  int debugX = 0;
  mvprintw(debugY, debugX, "Player x: %.2f y: %.2f angle: %.2f", player.get_x(),
           player.get_y(), player.get_angle());
  debugY = 1;
  mvprintw(debugY, debugX, "fps: %.2f", 1.f / delta / 1000);
}

std::tuple<int, int> NCursesRenderer::GetScreenSize() {
  return std::make_tuple(_screenWidth, _screenHeight);
}

void NCursesRenderer::SetScreenSize(int x, int y) {
  _screenWidth = x;
  _screenHeight = y;
  if (_screenBuffer) {
    delete[] _screenBuffer;
    _screenBuffer = nullptr;
  }
  _screenBuffer = new char[_screenWidth * _screenHeight];

  EngineState::GetInstance()->screenHeight = _screenHeight;
  EngineState::GetInstance()->screenWidth = _screenWidth;
}

NCursesRenderer::~NCursesRenderer() {
  endwin();
  delete[] _screenBuffer;
}

#endif // OS check end
