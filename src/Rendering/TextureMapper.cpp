#include "../Headers/TextureMapper.h"

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

TextureMapper::TextureMapper(std::string initTexture)
    : _textureMipMaps{initTexture} {
  _texWidth = initTexture.find('\n');
  _texHeight = _texWidth;
  GenerateTextureMask();
}

// Expects all textures of the same kind
// to be the same size, which is a reasonable constraint
// ALSO expects the first one to be the brightest,
// and the last one to be the faintest
TextureMapper::TextureMapper(std::vector<std::string> initTextureVec)
    : _textureMipMaps(initTextureVec) {
  Logger::GetInstance()->log("Starting TexMask generation...", LogType::TEXPREP,
                             LogLevel::INFO);
  Logger::forceFlush();
  _texWidth = initTextureVec[0].find('\n');
  _texHeight = _texWidth;
  GenerateTextureMask();
  Logger::GetInstance()->log("TexMask generation COMPLETE", LogType::TEXPREP,
                             LogLevel::INFO);
  Logger::forceFlush();
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

char TextureMapper::sampleNN(float u, float v, int distIdx) {
  distIdx = std::clamp(distIdx, 0, (int)_textureMipMaps.size() - 1);
  if (_texWidth <= 0 || _texHeight <= 0 || _textureMipMaps[distIdx].empty())
    return ' ';
  unsigned int x = static_cast<unsigned int>(u * (_texWidth - 1) + 0.5f);
  unsigned int y = static_cast<unsigned int>(v * (_texHeight - 1) + 0.5f);

  x = std::clamp(x, 0u, _texWidth - 1);
  y = std::clamp(y, 0u, _texHeight - 1);
  size_t idx = static_cast<size_t>(y) * static_cast<size_t>(_texWidth + 1) +
               static_cast<size_t>(x);
  if (idx >= _textureMipMaps[distIdx].size()) {
    return ' ';
  }
  return _textureMipMaps[distIdx][idx];
}

void TextureMapper::nxyInterpolationScale(unsigned int width,
                                          unsigned int height, float distance) {
  std::string out;
  out.resize((width + 1) * height);
  for (size_t y = 0; y < height; y++) {
    float v = (float)y / (float)(height - 1);
    for (size_t x = 0; x < width; x++) {
      float u = (float)x / (float)(width - 1);
      out[y * (width + 1) + x] = sampleNN(u, v, (int)(distance));
    }
    out[y * (width + 1) + width] = '\n';
  }
  _texHeight = height;
  _texWidth = width;
  _textureMipMaps[0] = std::move(out);
}

std::string TextureMapper::getTexColumnAt(unsigned int height, float hitpoint) {
  return getTexColumnAt(height, hitpoint, 0);
}
std::string TextureMapper::getTexColumnAt(unsigned int height, float hitpoint,
                                          float distance) {
  std::string ret = "";
  ret.reserve(height);
  Logger::GetInstance()->log(
      "getTexColumnAt: height=" + std::to_string(height) + " hitpoint=" +
          std::to_string(hitpoint) + " distance=" + std::to_string(distance) +
          " mipMapSize=" + std::to_string(_textureMipMaps.size()) + " texW=" +
          std::to_string(_texWidth) + " texH=" + std::to_string(_texHeight),
      LogType::TEXPREP, LogLevel::INFO);
  Logger::GetInstance()->forceFlush();
  for (size_t y = 0; y < height; y++) {
    float y_pos = (float)y / (float)height;
<<<<<<< HEAD
    ret.push_back(sampleNN(hitpoint, y_pos));
=======
    ret.push_back(sampleNN(hitpoint, y_pos, (int)(distance)));
>>>>>>> f92cf18 (Distance based shading is implemented all throughout. Remaining things are: passing the actual distance from the core, adjusting repeating scaling, generating the textures)
  }

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
