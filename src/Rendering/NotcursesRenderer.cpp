#include "../Headers/NotcursesRenderer.h"
#include <iostream>
#include <memory>
#include <notcurses/notcurses.h>
#include <string>
#include <vector>

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

  memcpy(_screenBuffer, newBuffer, _screenWidth * _screenHeight);

  ncplane_erase(notcurses_stdplane(_nc.get()));
  for (int y = 0; y < _screenHeight; y++) {
    ncplane_cursor_move_yx(notcurses_stdplane(_nc.get()), y, 0);
    std::string l(_screenBuffer + y * _screenWidth, _screenWidth);
    ncplane_putstr(notcurses_stdplane(_nc.get()), l.c_str());
  }
}
void NotcursesRenderer::PrintBuffer() const { notcurses_render(_nc.get()); }
void NotcursesRenderer::PrintDebugInfo(const std::vector<std::string> &nfo) {
  ncplane_erase(_debugPln);
  for (size_t i = 0; i < nfo.size(); i++) {
    ncplane_cursor_move_yx(_debugPln, i, 0);
    ncplane_putstr(_debugPln, nfo[i].c_str());
  }
}

vec2i NotcursesRenderer::screenSize() const {
  return {_screenWidth, _screenHeight};
}
int NotcursesRenderer::screenHeight() const { return _screenHeight; }
int NotcursesRenderer::screenWidth() const { return _screenWidth; }

NotcursesRenderer::~NotcursesRenderer() { delete[] _screenBuffer; }
