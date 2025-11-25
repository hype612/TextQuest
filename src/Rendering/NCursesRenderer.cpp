#include "../Headers/NCursesRenderer.h"

#if (defined(LINUX) || defined(__linux__))

NCursesRenderer::NCursesRenderer() { Init(); }

void NCursesRenderer::Init() {
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  _screenHeight = EngineState::GetInstance()->screenHeight;
  _screenWidth = EngineState::GetInstance()->screenWidth;
  _screenBuffer = nullptr;
}

void NCursesRenderer::OverwriteBuffer(wchar_t *newBuffer) {
  if (_screenBuffer == nullptr) {
    std::cerr << "ERROR: no screenbuffer has been initialized yet. Please call "
                 "SetScreenSize before calling OverWriteBuffer"
              << std::endl;
  }
  _screenBuffer = newBuffer;
  addwstr(_screenBuffer);
}

void NCursesRenderer::PrintBuffer() { refresh(); }

std::tuple<int, int> NCursesRenderer::GetScreenSize() {
  return std::make_tuple(_screenWidth, _screenHeight);
}

void NCursesRenderer::SetScreenSize(int x, int y) {
  _screenWidth = x;
  _screenHeight = y;
  delete[] _screenBuffer;
  _screenBuffer = new wchar_t[_screenWidth * _screenHeight];

  EngineState::GetInstance()->screenHeight = _screenHeight;
  EngineState::GetInstance()->screenWidth = _screenWidth;
}

NCursesRenderer::~NCursesRenderer() {
  endwin();
  delete[] _screenBuffer;
}

#endif // OS check end
