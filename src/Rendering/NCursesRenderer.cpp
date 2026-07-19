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

void NCursesRenderer::PrintBuffer() const { refresh(); }

void NCursesRenderer::PrintDebugInfo(const std::vector<std::string> &nfo) {
  for (size_t i = 0; i < nfo.size(); i++) {
    mvprintw(i, 0, "%s", nfo[i].c_str());
  }
}

std::tuple<int, int> NCursesRenderer::GetScreenSize() const {
  return std::make_tuple(_screenWidth, _screenHeight);
}

NCursesRenderer::~NCursesRenderer() {
  endwin();
  delete[] _screenBuffer;
}

#endif // OS check end
