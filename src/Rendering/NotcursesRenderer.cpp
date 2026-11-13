#include "../Headers/NotcursesRenderer.h"
#include "IRenderer.h"
#include "Logger.h"
#include <iostream>
#include <memory>
#include <notcurses/notcurses.h>
#include <stdexcept>
#include <string>
#include <vector>

NotcursesRenderer::NotcursesRenderer(std::shared_ptr<notcurses> nc) : _nc(nc) {
  _screenHeight = ncplane_dim_y(notcurses_stdplane(_nc.get()));
  _screenWidth = ncplane_dim_x(notcurses_stdplane(_nc.get()));
  ncplane_set_scrolling(notcurses_stdplane(_nc.get()), false);
  _screenBuffer = new char[_screenWidth * _screenHeight];
}

void NotcursesRenderer::Init() {}
void NotcursesRenderer::OverwriteBuffer(char *newBuffer) {
  if (newBuffer == nullptr) {
    Logger::GetInstance()->log(
        "OverWriteBuffer: function was called with nullptr", LogType::RENDER,
        LogLevel::ERROR);
    throw(std::invalid_argument(
        "OverWriteBuffer: function was called with nullptr"));
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
vec2i NotcursesRenderer::screenSize() const {
  return {_screenWidth, _screenHeight};
}
int NotcursesRenderer::screenHeight() const { return _screenHeight; }
int NotcursesRenderer::screenWidth() const { return _screenWidth; }

OverlayId NotcursesRenderer::createOverlay(Rect area) {
  if (area.x <= 0 || area.y <= 0) {
    Logger::GetInstance()->log(
        "CreateOverlay: x or y of the new rectangle was a negative value, "
        "skipping the creation. x: " +
            std::to_string(area.x) + " y: " + std::to_string(area.y),
        LogType::RENDER, LogLevel::ERROR);
    return -1;
  }

  if (area.width <= 0 || area.height <= 0) {
    Logger::GetInstance()->log("CreateOverlay: width or height of the new "
                               "rectangle was a negative value, "
                               "skipping the creation. width: " +
                                   std::to_string(area.width) +
                                   " height: " + std::to_string(area.height),
                               LogType::RENDER, LogLevel::ERROR);
    return -1;
  }
  for (auto &[id, ov] : _overlays) {
    if (ov.rect.overlaps(area)) {
      Logger::GetInstance()->log(
          "CreateOverlay: The proposed plane overlaps with an existing one.",
          LogType::RENDER, LogLevel::ERROR);
      return -1;
    }
  }

  ncplane_options opts{area.y, area.x, area.height, area.width, NULL,
                       NULL,   NULL,   0,           0,          0};
  ncplane *plane = ncplane_create(notcurses_stdplane(_nc.get()), &opts);
  if (!plane) {
    // The arguments were valid, so this is notcurses failing, not the caller.
    Logger::GetInstance()->log("CreateOverlay: ncplane_create failed.",
                               LogType::RENDER, LogLevel::ERROR);
    throw std::runtime_error("CreateOverlay: ncplane_create failed.");
  }
  _topId++;
  _overlays[_topId] = Overlay{plane, area};
  return _topId;
}

bool NotcursesRenderer::destroyOverlay(OverlayId id) {
  auto it = _overlays.find(id);
  if (it == _overlays.end()) {
    Logger::GetInstance()->log(
        "destroyOverlay: function was called with invalid id.", LogType::RENDER,
        LogLevel::ERROR);
    return false;
  }
  ncplane_destroy(it->second.plane);
  _overlays.erase(it);
  return true;
}

// notcurses has no plane hide, but planes entirely outside the screen are not
// rendered, so hiding parks the plane just below the bottom edge.
bool NotcursesRenderer::setOverlayVisible(OverlayId id, bool visible) {
  auto it = _overlays.find(id);
  if (it == _overlays.end()) {
    Logger::GetInstance()->log(
        "setOverlayVisible: function was called with invalid id.",
        LogType::RENDER, LogLevel::ERROR);
    return false;
  }
  const Rect &r = it->second.rect;
  return ncplane_move_yx(it->second.plane, visible ? r.y : _screenHeight,
                         r.x) == 0;
}

// Sets the plane's active colors, which every later write on the plane picks
// up. Existing cells keep the colors they were written with.
static void applyColors(ncplane *plane, std::optional<uint32_t> fg,
                        std::optional<uint32_t> bg) {
  if (fg)
    ncplane_set_fg_rgb(plane, *fg);
  else
    ncplane_set_fg_default(plane);
  if (bg)
    ncplane_set_bg_rgb(plane, *bg);
  else
    ncplane_set_bg_default(plane);
}

void NotcursesRenderer::putRow(ncplane *plane, int y, int x,
                               const std::string &text, size_t width,
                               bool transparentSpaces) {
  const size_t len = std::min(text.size(), width);
  if (!transparentSpaces) {
    ncplane_cursor_move_yx(plane, y, x);
    ncplane_putstr(plane, text.substr(0, len).c_str());
    return;
  }
  for (size_t i = 0; i < len; i++) {
    if (text[i] != ' ')
      ncplane_putchar_yx(plane, y, x + static_cast<int>(i), text[i]);
  }
}

void NotcursesRenderer::setOverlay(OverlayId id,
                                   const std::vector<std::string> &content,
                                   std::optional<uint32_t> fg,
                                   std::optional<uint32_t> bg,
                                   bool transparentSpaces) {
  ncplane *current = _overlays.at(id).plane;
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
    throw(std::invalid_argument("SetOverlaycontent: Content size did not match "
                                "the area size. OverlayId: " +
                                std::to_string(id)));
  }

  ncplane_erase(current);
  applyColors(current, fg, bg);
  int printheight = (content.size() < p_height) ? content.size() : p_height;
  for (int y = 0; y < printheight; y++)
    putRow(current, y, 0, content[y], p_width, transparentSpaces);
}

// region x,y should be relative to the parent ncplane == ui element
void NotcursesRenderer::setOverlayRegion(
    OverlayId id, Rect region, const std::vector<std::string> &content,
    std::optional<uint32_t> fg, std::optional<uint32_t> bg,
    bool transparentSpaces) {
  if (region.width == 0 || region.height == 0) {
    Logger::GetInstance()->log(
        "setOverlayRegion: fn was called with 0 height or width region",
        LogType::RENDER, LogLevel::ERROR);
    throw(std::invalid_argument(
        "setOverlayRegion: fn was called with 0 height or width region"));
  }

  ncplane *current = _overlays.at(id).plane;
  // check if rect is completely in plane
  Rect target{0, 0, 0, 0};
  ncplane_dim_yx(current, &target.height, &target.width);
  if (!target.contains(region)) {
    Logger::GetInstance()->log(
        "setOverlayRegion: overwrite region is not fully in target region.",
        LogType::RENDER, LogLevel::ERROR);
    throw(std::invalid_argument(
        "setOverlayRegion: overwrite region is not fully in target region."));
  }

  // roughly check if the new content fits the rect
  int c_len = 0;
  for (const auto &s : content) {
    c_len += s.length();
  }
  if (c_len > (static_cast<int>(region.width * region.height))) {
    Logger::GetInstance()->log("setOverlayRegion: Content size did not match "
                               "the area size. OverlayId: " +
                                   std::to_string(id),
                               LogType::RENDER, LogLevel::ERROR);
    throw(std::invalid_argument("setOverlayRegion: Content size did not match "
                                "the area size. OverlayId: " +
                                std::to_string(id)));
  }

  // override region
  if (transparentSpaces)
    ncplane_erase_region(current, region.y, region.x, region.height,
                         region.width);
  applyColors(current, fg, bg);
  int printheight =
      (content.size() < region.height) ? content.size() : region.height;
  for (int y = 0; y < printheight; y++)
    putRow(current, region.y + y, region.x, content[y], region.width,
           transparentSpaces);
}

NotcursesRenderer::~NotcursesRenderer() {
  delete[] _screenBuffer;
  for (auto &[id, ov] : _overlays) {
    ncplane_destroy(ov.plane);
  }
}
