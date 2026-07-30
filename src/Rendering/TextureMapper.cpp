#include "../Headers/TextureMapper.h"
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

TextureMapper::TextureMapper(std::string initTexture)
    : _textureMipMaps{initTexture} {
  _texWidth = initTexture.find('\n');
  _texHeight = initTexture.size() / (_texWidth + 1);
  GenerateTextureMask();
}

TextureMapper::TextureMapper(std::vector<std::string> initTextureVec)
    : _textureMipMaps(initTextureVec) {
  _texWidth = initTextureVec[0].find('\n');
  _texHeight = initTextureVec[0].size() / (_texWidth + 1);
  GenerateTextureMask();
}

void TextureMapper::GenerateTextureMask() {
  _textureMask.clear();
  for (size_t i = 0; i < _texHeight; i++) {
    for (size_t j = 0; j < _texWidth; j++) {
      if (i * (_texWidth + 1) + j >= _textureMipMaps[0].size())
        break;
      if (_textureMipMaps[0][i * (_texWidth + 1) + j] == ' ')
        _textureMask.push_back(0);
      else if (_textureMipMaps[0][i * (_texWidth + 1) + j] == '\n')
        _textureMask.push_back(-1);
      else
        _textureMask.push_back(1);
    }
  }
}

void TextureMapper::repeatingHorizontalDownscale(unsigned int width) {
  std::string tex = "";
  tex.reserve(_texHeight * (width + 1));
  if (_texWidth > width) {
    for (size_t i = 0; i < _texHeight; i++) {
      tex.append(_textureMipMaps[0].substr(i * _texWidth + i, width));
      tex.push_back(L'\n');
    }
  }
  _texWidth = width;
  _textureMipMaps[0] = std::move(tex);
}

void TextureMapper::repeatingVerticalDownscale(unsigned int height,
                                               unsigned int width) {
  _textureMipMaps[0] = _textureMipMaps[0].substr(0, width * height + height);
  _texHeight = height;
}

void TextureMapper::repeatingHorizontalUpscale(unsigned int height,
                                               unsigned int width) {
  std::string line = "";
  line.reserve(width);
  std::string new_tex = "";
  new_tex.reserve(height * width);

  for (size_t i = 0; i < _texHeight; i++) {
    line = _textureMipMaps[0].substr((1 + _texWidth) * i, _texWidth);
    while (line.length() < width)
      line.append(line.substr(0, width - line.length()));
    new_tex.append(line);
    new_tex.push_back(L'\n');
  }
  _textureMipMaps[0] = std::move(new_tex);
  _texWidth = width;
}

void TextureMapper::repeatingVerticalUpscale(unsigned int height) {
  std::string new_tex = _textureMipMaps[0];
  int i = 0;
  while (_texHeight < height) {
    new_tex += new_tex.substr((_texWidth + 1) * i, _texWidth);
    i = (i + 1) % _texHeight;
    _texHeight++;
  }
  _textureMipMaps[0] = std::move(new_tex);
}

const std::string &TextureMapper::getTexture() const {
  return _textureMipMaps[0];
}
const std::vector<int> &TextureMapper::getMask() const { return _textureMask; }

// =======================
//  interpolation scaling
// =======================

char TextureMapper::sampleNN(float u, float v, int shadingIdx) const {
  shadingIdx = std::clamp(shadingIdx, 0, (int)_textureMipMaps.size() - 1);
  if (_texWidth <= 0 || _texHeight <= 0 || _textureMipMaps[shadingIdx].empty())
    return ' ';
  unsigned int x = static_cast<unsigned int>(u * (_texWidth - 1) + 0.5f);
  unsigned int y = static_cast<unsigned int>(v * (_texHeight - 1) + 0.5f);

  x = std::clamp(x, 0u, _texWidth - 1);
  y = std::clamp(y, 0u, _texHeight - 1);
  size_t idx = static_cast<size_t>(y) * static_cast<size_t>(_texWidth + 1) +
               static_cast<size_t>(x);
  if (idx >= _textureMipMaps[shadingIdx].size()) {
    return ' ';
  }
  return _textureMipMaps[shadingIdx][idx];
}

int TextureMapper::samepleNNonMask(float u, float v) const {
  if (_texWidth <= 0 || _texHeight <= 0)
    return 0;
  unsigned int x = static_cast<unsigned int>(u * (_texWidth - 1) + 0.5f);
  unsigned int y = static_cast<unsigned int>(v * (_texHeight - 1) + 0.5f);

  x = std::clamp(x, 0u, _texWidth - 1);
  y = std::clamp(y, 0u, _texHeight - 1);
  size_t idx = static_cast<size_t>(y) * static_cast<size_t>(_texWidth + 1) +
               static_cast<size_t>(x);
  if (idx >= _textureMask.size()) {
    return 0;
  }
  return _textureMask[idx];
}

std::string TextureMapper::nxyInterpolationScale(unsigned int width,
                                                 unsigned int height,
                                                 int shadingIdx) const {
  std::string out;

  out.resize((width + 1) * height);
  for (size_t y = 0; y < height; y++) {
    float v = (height > 1) ? (float)y / (float)(height - 1) : 0.f;
    for (size_t x = 0; x < width; x++) {
      float u = (width > 1) ? (float)x / (float)(width - 1) : 0.f;
      out[y * (width + 1) + x] = sampleNN(u, v, shadingIdx);
    }
    out[y * (width + 1) + width] = '\n';
  }
  return out;
}

std::string TextureMapper::scaledTex(unsigned int width, unsigned int height,
                                     int shadingIdx) const {
  return nxyInterpolationScale(width, height, shadingIdx);
}

std::vector<int> TextureMapper::scaledMask(unsigned int width,
                                           unsigned int height) const {
  std::vector<int> out;

  out.resize((width + 1) * height);
  for (size_t y = 0; y < height; y++) {
    float v = (height > 1) ? (float)y / (float)(height - 1) : 0.f;
    for (size_t x = 0; x < width; x++) {
      float u = (width > 1) ? (float)x / (float)(width - 1) : 0.f;
      out[y * (width + 1) + x] = samepleNNonMask(u, v);
    }
    out[y * (width + 1) + width] = '\n';
  }
  return out;
}

std::string TextureMapper::getTexColumnAt(unsigned int height, float hitpoint,
                                          int visibleTop,
                                          int visibleBot) const {
  return getTexColumnAt(height, hitpoint, visibleTop, visibleBot, 0);
}
std::string TextureMapper::getTexColumnAt(unsigned int height, float hitpoint,
                                          int visibleTop, int visibleBot,
                                          int shadingIdx) const {
  std::string ret = "";
  ret.reserve(height);
  for (int y = visibleTop; y < visibleBot; y++) {
    float y_pos = (float)y / (float)height;
    ret.push_back(sampleNN(hitpoint, y_pos, shadingIdx));
  }

  return ret;
}

std::vector<int> TextureMapper::getMaskColumnAt(unsigned int height,
                                                float hitpoint) const {
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
