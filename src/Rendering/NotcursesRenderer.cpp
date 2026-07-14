#include "../Headers/NotcursesRenderer.h"
#include <memory>
#include <notcurses/notcurses.h>
#include <string>

NotcursesRenderer::NotcursesRenderer(std::shared_ptr<notcurses> nc) : _nc(nc) {
  _screenHeight = ncplane_dim_y(notcurses_stdplane(_nc.get()));
  _screenWidth = ncplane_dim_x(notcurses_stdplane(_nc.get()));
  ncplane_set_scrolling(notcurses_stdplane(_nc.get()), false);
  ncplane_options ncpopts{0,
                          0,
                          ncplane_dim_y(notcurses_stdplane(_nc.get())),
                          ncplane_dim_x(notcurses_stdplane(_nc.get())),
                          NULL,
                          "debug",
                          NULL,
                          NCPLANE_OPTION_FIXED,
                          0,
                          0};
  _debugPln = ncplane_create(notcurses_stdplane(_nc.get()), &ncpopts);
  _screenBuffer = new char[_screenWidth * _screenHeight];
}

void NotcursesRenderer::Init() {}
void NotcursesRenderer::OverwriteBuffer(char *newBuffer) {
  if (newBuffer == nullptr) {
    std::cerr << "OverWriteBuffer: function was called with nullptr"
              << std::endl;
    return;
  }

  _screenBuffer = newBuffer;

  ncplane_erase(notcurses_stdplane(_nc.get()));
  for (int y = 0; y < _screenHeight; y++) {
    ncplane_cursor_move_yx(notcurses_stdplane(_nc.get()), y, 0);
    std::string l(_screenBuffer + y * _screenWidth, _screenWidth);
    ncplane_putstr(notcurses_stdplane(_nc.get()), l.c_str());
  }
}
void NotcursesRenderer::PrintBuffer() { notcurses_render(_nc.get()); }
void NotcursesRenderer::PrintDebugInfo(const Player &player, float delta) {
  if (EngineState::GetInstance()->renderDebugInfo) {
    ncplane_cursor_move_yx(_debugPln, 0, 0);
    ncplane_putstr(_debugPln, ("P.x:" + std::to_string(player.getX()) +
                               " P.y: " + std::to_string(player.getY()) +
                               " angle: " + std::to_string(player.getAngle()))
                                  .c_str());
    ncplane_cursor_move_yx(_debugPln, 1, 0);
    ncplane_putstr(_debugPln, ("fps: " + std::to_string(1.f / delta)).c_str());
  } else {
    ncplane_erase(_debugPln);
  }
}
std::tuple<int, int> NotcursesRenderer::GetScreenSize() {
  return std::tuple<int, int>(_screenWidth, _screenHeight);
}
NotcursesRenderer::~NotcursesRenderer() { delete[] _screenBuffer; }
