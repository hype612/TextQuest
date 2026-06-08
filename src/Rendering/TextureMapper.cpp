#include "../Headers/TextureMapper.h"

#include <algorithm>
#include <cstddef>
#include <string>

TextureMapper::TextureMapper(std::string initTexture)
    : _textureMipMap(initTexture) {
  GenerateTextureMask();
  _texWidth = initTexture.find('\n');
  _texHeight = _texWidth;
}

float TextureMapper::estimateHeight(float distance) {
  if (distance > 9.f)
    return 0;
  return (-(distance / 0.105f) + 98.2f);
}
float TextureMapper::estimateWidth(float distance) {
  if (distance > 9.f)
    return 0;
  return (-(distance / 0.0375f) + 294.f);
}

// naive mask generator
// technically works, but later when its
// shading time, the most bright pixels wont be shaded
// if the shading is dependant of the mask
void TextureMapper::GenerateTextureMask() {
  _textureMask.clear();
  unsigned int tx_width = _textureMipMap.find('\n');
  unsigned int tx_height = tx_width;
  for (size_t i = 0; i < tx_height; i++) {
    for (size_t j = 0; j < tx_width; j++) {
      if (i * (tx_width + 1) + j >= _textureMipMap.size())
        break;
      if (_textureMipMap[i * (tx_width + 1) + j] == ' ')
        _textureMask.push_back(0);
      else if (_textureMipMap[i * (tx_width + 1) + j] == '\n')
        _textureMask.push_back(-1);
      else
        _textureMask.push_back(1);
    }
  }
}

void TextureMapper::setCurrentTexture(float distance, const std::string &mode,
                                      std::string *tex) {
  unsigned int height = (unsigned int)(estimateHeight(distance));
  unsigned int width = (unsigned int)(estimateWidth(distance));
  _stepper = 0;
  std::string line = *tex;
  _texWidth = line.find('\n');
  _texHeight = std::count(line.begin(), line.end(), '\n');
  _textureMipMap = *tex;
  Logger::GetInstance()->log("setCurrentTexture: starting scaling to " +
                                 std::to_string(_texWidth) + "x" +
                                 std::to_string(_texHeight),
                             LogType::TEXPREP, LogLevel::INFO);

  // HORIZONTAL SCALE
  if (mode == "repeat") {
    if (_texWidth < width) {
      repeatingHorizontalUpscale(height, width);
    }
    if (_texWidth > width) {
      repeatingHorizontalDownscale(width);
    }
    // VERTICAL SCALE
    if (_texHeight < height) {
      repeatingVerticalUpscale(height);
    }
    if (_texHeight > height) {
      repeatingVerticalDownscale(height, width);
    }
  } else if (mode == "interpolating") {
    if (_texWidth != width || _texHeight != height) {
      nxyInterpolationScale(width, height);
    }
    /*
    if (_texWidth < width) {
      nxInterpolationUpscale(width);
    }
    if (_texWidth > width) {
      nxInterpolationDownscale(width);
    }
    // VERTICAL SCALE
    if (_texHeight < height) {
      nyInterpolationUpscale(height);
    }
    if (_texHeight > height) {
      nyInterpolationDownscale(height);
    }*/
  }
  Logger::GetInstance()->log("generated texture:", LogType::TEXPREP,
                             LogLevel::INFO);
  Logger::GetInstance()->log(_textureMipMap, LogType::TEXPREP, LogLevel::INFO);
  Logger::GetInstance()->log("=====================================\n\n",
                             LogType::TEXPREP, LogLevel::INFO);
  GenerateTextureMask();
}

void TextureMapper::rescaleCurrentTexture(float distance) {
  setCurrentTexture(distance, "interpolating", &_textureMipMap);
}

void TextureMapper::repeatingHorizontalDownscale(unsigned int width) {
  std::string tex = "";
  tex.reserve(_texHeight * (width + 1));
  if (_texWidth > width) {
    for (size_t i = 0; i < _texHeight; i++) {
      tex.append(_textureMipMap.substr(i * _texWidth + i, width));
      tex.push_back(L'\n');
    }
  }
  _texWidth = width;
  _textureMipMap = std::move(tex);
}

void TextureMapper::repeatingVerticalDownscale(unsigned int height,
                                               unsigned int width) {
  _textureMipMap = _textureMipMap.substr(0, width * height + height);
  _texHeight = height;
}

void TextureMapper::repeatingHorizontalUpscale(unsigned int height,
                                               unsigned int width) {
  std::string line = "";
  line.reserve(width);
  std::string new_tex = "";
  new_tex.reserve(height * width);

  for (size_t i = 0; i < _texHeight; i++) {
    line = _textureMipMap.substr((1 + _texWidth) * i, _texWidth);
    while (line.length() < width)
      line.append(line.substr(0, width - line.length()));
    new_tex.append(line);
    new_tex.push_back(L'\n');
  }
  _textureMipMap = std::move(new_tex);
  _texWidth = width;
}

void TextureMapper::repeatingVerticalUpscale(unsigned int height) {
  std::string new_tex = _textureMipMap;
  int i = 0;
  while (_texHeight < height) {
    new_tex += new_tex.substr((_texWidth + 1) * i, _texWidth);
    i = (i + 1) % _texHeight;
    _texHeight++;
  }
  _textureMipMap = std::move(new_tex);
}

const std::string &TextureMapper::getTexture() const { return _textureMipMap; }
const std::vector<int> &TextureMapper::getMask() const { return _textureMask; }

// =======================
//  interpolation scaling
// =======================

char TextureMapper::sampleNN(float u, float v) {
  if (_texWidth <= 0 || _texHeight <= 0 || _textureMipMap.empty())
    return ' ';
  unsigned int x = static_cast<unsigned int>(u * (_texWidth - 1) + 0.5f);
  unsigned int y = static_cast<unsigned int>(v * (_texHeight - 1) + 0.5f);

  x = std::clamp(x, 0u, _texWidth - 1);
  y = std::clamp(y, 0u, _texHeight - 1);
  size_t idx = static_cast<size_t>(y) * static_cast<size_t>(_texWidth + 1) +
               static_cast<size_t>(x);
  if (idx >= _textureMipMap.size()) {
    return ' ';
  }
  return _textureMipMap[idx];
}

void TextureMapper::nxyInterpolationScale(unsigned int width,
                                          unsigned int height) {
  std::string out;
  out.resize((width + 1) * height);
  for (size_t y = 0; y < height; y++) {
    float v = (float)y / (float)(height - 1);
    for (size_t x = 0; x < width; x++) {
      float u = (float)x / (float)(width - 1);
      out[y * (width + 1) + x] = sampleNN(u, v);
    }
    out[y * (width + 1) + width] = '\n';
  }
  _texHeight = height;
  _texWidth = width;
  _textureMipMap = std::move(out);
}

void TextureMapper::nxInterpolationDownscale(unsigned int width) {
  /*
  std::string new_tex;
  new_tex.reserve(width * _texHeight);

  float x_ratio = static_cast<float>(_texWidth) / width;

  for (int y = 0; y < _texHeight; y++) {
    for (int x = 0; x < width; x++) {
      int src_x = static_cast<int>(x * x_ratio);
      char pb = _textureMipMap[y * (_texWidth + 1) + src_x];
      if (pb != '\n')
        new_tex.push_back(pb);
      else
        new_tex.push_back(_textureMipMap[y * (_texWidth + 1) + src_x - 1]);
    }
    new_tex.push_back(L'\n');
  }
  _textureMipMap = std::move(new_tex);
  _texWidth = width;*/
  std::string out;
  out.resize((width + 1) * _texHeight);
  for (size_t y = 0; y < _texHeight; y++) {
    for (size_t x = 0; x < width; x++) {
      float u = (float)x / (float)(width - 1);
      out[y * (width + 1) + x] = sampleNN(u, (float)y / (_texHeight - 1));
    }
    out[y * (width + 1) + width] = '\n';
  }
  _textureMipMap = std::move(out);
  _texWidth = width;
}

void TextureMapper::nxInterpolationUpscale(unsigned int width) {
  std::string out;
  out.resize((width + 1) * _texHeight);
  for (size_t y = 0; y < _texHeight; y++) {
    for (size_t x = 0; x < width; x++) {
      float u = (float)x / (float)(width - 1);
      out[y * (width + 1) + x] = sampleNN(u, (float)y / (_texHeight - 1));
    }
    out[y * (width + 1) + width] = '\n';
  }
  _textureMipMap = std::move(out);
  _texWidth = width;
}

void TextureMapper::nyInterpolationDownscale(unsigned int height) {
  /*std::string new_tex;
  new_tex.reserve(_texWidth * height);

  float y_ratio = static_cast<float>(_texHeight) / height;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < _texWidth; x++) {
      int src_y = static_cast<int>(y * y_ratio);
      char pb = _textureMipMap[src_y * (_texWidth + 1) + x];
      if (pb != '\n')
        new_tex.push_back(pb);
      else
        new_tex.push_back(_textureMipMap[src_y * (_texWidth + 1) + x - 1]);
    }
    new_tex.push_back('\n');
  }
  _textureMipMap = std::move(new_tex);
  _texHeight = height;*/
  std::string out;
  out.resize((_texWidth + 1) * height);
  for (size_t y = 0; y < height; y++) {
    float v = (float)y / (float)(height - 1);
    for (size_t x = 0; x < _texWidth; x++) {
      float u = (float)x / (float)(_texWidth - 1);
      out[y * (_texWidth + 1) + x] = sampleNN(u, v);
    }
    out[y * (_texWidth + 1) + _texWidth] = '\n';
  }
  _textureMipMap = std::move(out);
  _texHeight = height;
}

void TextureMapper::nyInterpolationUpscale(unsigned int height) {
  std::string out;
  out.resize((_texWidth + 1) * height);
  for (size_t y = 0; y < height; y++) {
    float v = (float)y / (float)(height - 1);
    for (size_t x = 0; x < _texWidth; x++) {
      float u = (float)x / (float)(_texWidth - 1);
      out[y * (_texWidth + 1) + x] = sampleNN(u, v);
    }
    out[y * (_texWidth + 1) + _texWidth] = '\n';
  }
  _textureMipMap = std::move(out);
  _texHeight = height;
}

std::string TextureMapper::ScaleToHeight(unsigned int height,
                                         std::string column) {
  std::string r_column = "";
  r_column.reserve(height);
  while (r_column.length() < height) {
    for (char c : column) {
      r_column.push_back(c);
    }
  }
  return r_column.substr(0, height);
}
std::string TextureMapper::getTextColumnAt(unsigned int height,
                                           float hitpoint) {
  std::string ret = "";
  ret.reserve(height);
  for (size_t y = 0; y < height; y++) {
    float y_pos = (float)y / (float)_texHeight;
    ret.push_back(sampleNN(hitpoint, y_pos));
  }

  Logger::GetInstance()->log("getTexColumnAt: the returned col for height: " +
                                 std::to_string(height),
                             LogType::RENDER, LogLevel::INFO);
  Logger::GetInstance()->log(ret, LogType::RENDER, LogLevel::INFO);
  _stepper++;
  return ret;
}
std::vector<int> TextureMapper::getMaskColumnAt(unsigned int height,
                                                float hitpoint) {
  std::vector<int> ret;
  ret.reserve(height);
  unsigned int x = static_cast<unsigned int>(hitpoint * (_texWidth - 1) + 0.5f);
  x = std::clamp(x, 0u, _texWidth - 1);
  for (size_t y = 0; y < height; y++) {
    int ty = (y % _texHeight);
    ret.push_back(_textureMask[ty * _texWidth + x]);
  }
  return ret;
}
