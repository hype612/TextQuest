#include "../Headers/NotcursesRenderer.h"
#include "Logger.h"
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
  _debugPlane = ncplane_create(notcurses_stdplane(_nc.get()), &ncpopts);
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
  ncplane_erase(_debugPlane);
  for (size_t i = 0; i < nfo.size(); i++) {
    ncplane_cursor_move_yx(_debugPlane, i, 0);
    ncplane_putstr(_debugPlane, nfo[i].c_str());
  }
}

vec2i NotcursesRenderer::screenSize() const {
  return {_screenWidth, _screenHeight};
}
int NotcursesRenderer::screenHeight() const { return _screenHeight; }
int NotcursesRenderer::screenWidth() const { return _screenWidth; }

OverlayId NotcursesRenderer::createOverlay(Rect area) {
  if (area.x <= 0 || area.y <= 0) {
    Logger::GetInstance()->log(
        "CreatePopupArea: x or y of the new rectangle was a negative value, "
        "skipping the creation. x: " +
            std::to_string(area.x) + " y: " + std::to_string(area.y),
        LogType::RENDER, LogLevel::ERROR);
    return -1;
  }

  if (area.width <= 0 || area.height <= 0) {
    Logger::GetInstance()->log("CreatePopupArea: width or height of the new "
                               "rectangle was a negative value, "
                               "skipping the creation. width: " +
                                   std::to_string(area.width) +
                                   " height: " + std::to_string(area.height),
                               LogType::RENDER, LogLevel::ERROR);
    return -1;
  }
  for (auto &[id, pln] : _overlayPlanes) {
    Rect r{ncplane_x(pln), ncplane_y(pln), ncplane_dim_x(pln),
           ncplane_dim_y(pln)};
    if (r.overlaps(area)) {
      Logger::GetInstance()->log(
          "CreatePopupArea: The proposed plane overlaps with an existing one.",
          LogType::RENDER, LogLevel::ERROR);
      return -1;
    }
  }

  _topId++;
  ncplane_options opts{area.y, area.x, area.height, area.width, NULL,
                       NULL,   NULL,   0,           0,          0};
  _overlayPlanes[_topId] = ncplane_create(notcurses_stdplane(_nc.get()), &opts);
  return _topId;
}
void NotcursesRenderer::setOverlayContent(
    OverlayId id, const std::vector<std::string> &content) {

  ncplane *current = _overlayPlanes.at(id);
  unsigned int p_width, p_height;
  int p_x, p_y;
  ncplane_dim_yx(current, &p_height, &p_width);
  ncplane_yx(current, &p_y, &p_x);

  int c_len = 0;
  for (const auto &s : content) {
    c_len += s.length();
  }
  if (c_len > (static_cast<int>(p_width * p_height))) {
    Logger::GetInstance()->log("SetOverlaycontent: Content size did not match "
                               "the area size. OverlayId: " +
                                   std::to_string(id),
                               LogType::RENDER, LogLevel::ERROR);
  }

  ncplane_erase(current);
  int printheight = (content.size() < p_height) ? content.size() : p_height;
  for (int y = 0; y < printheight; y++) {
    ncplane_cursor_move_yx(current, y, 0);
    ncplane_putstr(current, content[y].substr(0, p_width).c_str());
  }
}

void NotcursesRenderer::setOverlaRegion(
    OverlayId id, Rect region, const std::vector<std::string> &content) {
  ncplane *current = _overlayPlanes.at(id);
  unsigned int p_width, p_height;
  int p_x, p_y;
  ncplane_dim_yx(current, &p_height, &p_width);
  ncplane_yx(current, &p_y, &p_x);

  int c_len = 0;
  for (const auto &s : content) {
    c_len += s.length();
  }
  if (c_len > (static_cast<int>(p_width * p_height))) {
    Logger::GetInstance()->log("SetOverlaycontent: Content size did not match "
                               "the area size. OverlayId: " +
                                   std::to_string(id),
                               LogType::RENDER, LogLevel::ERROR);
  }

  ncplane_erase(current);
  int printheight = (content.size() < p_height) ? content.size() : p_height;
  for (int y = 0; y < printheight; y++) {
    ncplane_cursor_move_yx(current, y, 0);
    ncplane_putstr(current, content[y].substr(0, p_width).c_str());
  }
}

NotcursesRenderer::~NotcursesRenderer() {
  delete[] _screenBuffer;
  for (auto &[id, plane] : _overlayPlanes) {
    ncplane_destroy(plane);
  }
}
