#include "../Headers/NotcursesRenderer.h"
#include "Logger.h"
#include "RenderAssetManager.h"
#include <iostream>
#include <memory>
#include <notcurses/notcurses.h>
#include <string>
#include <utility>
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

  _hudPlane = ncplane_create(notcurses_stdplane(_nc.get()), &ncpopts);
  _popupPlane = ncplane_create(notcurses_stdplane(_nc.get()), &ncpopts);
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

OverlayId NotcursesRenderer::createHudArea(Rect area) {
  for (const auto &[id, area_nfo] : _overdrawArea) {
    const auto &[pln, r] = area_nfo;
    if (pln == RenderPlane::HUD && r.overlaps(area)) {
      return -1;
    }
  }

  _topId++;
  _overdrawArea[_topId] = std::make_pair(RenderPlane::HUD, area);
  return _topId;
}
OverlayId NotcursesRenderer::createPopupArea(Rect area) {
  // check overlap on same layer
  for (const auto &[id, area_nfo] : _overdrawArea) {
    const auto &[pln, r] = area_nfo;
    if (pln == RenderPlane::POPUP && r.overlaps(area)) {
      return -1;
    }
  }

  _topId++;
  _overdrawArea[_topId] = std::make_pair(RenderPlane::POPUP, area);
  return _topId;
  // add or log and decline if necessary
}
void NotcursesRenderer::setOverlayContent(
    OverlayId id, const std::vector<const std::string> &content) {

  // TODO: ERASE ALL PLANES SOMEWHERE
  // BEFORE IT BITES YOU IN THE ASS
  const std::pair<RenderPlane, Rect> &current = _overdrawArea[id];
  // test for id

  // test content lengte
  if (content.size() > current.second.width * current.second.height) {
    Logger::GetInstance()->log("SetOverlaycontent: Content size did not match "
                               "the area size. OverlayId: " +
                                   std::to_string(id),
                               LogType::RENDER, LogLevel::ERROR);
    return;
  }
  vec2i cursor = {current.second.x, current.second.y};
  ncplane *active_plane;
  switch (current.first) {
  case RenderPlane::HUD:
    active_plane = _hudPlane;
    break;
  case RenderPlane::POPUP:
    active_plane = _popupPlane;
    break;
  }

  ncplane_cursor_move_yx(active_plane, cursor.y, cursor.x);
  // ncplane_putstr(_debugPlane, nfo[i].c_str());
}
}

NotcursesRenderer::~NotcursesRenderer() { delete[] _screenBuffer; }
