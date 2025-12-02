#include "../Headers/TextureMapper.h"

TextureMapper::TextureMapper(std::string initTexture)
    : _textureMipMap(initTexture) {
  GenerateTextureMask();
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
  int tx_width = _textureMipMap.find('\n');
  int tx_height = tx_width;
  for (int i = 0; i < tx_height; i++) {
    for (int j = 0; j < tx_width; j++) {
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
  _stepper = 0;
  int height = (int)(estimateHeight(distance));
  int width = (int)(estimateWidth(distance));
  _stepper = 0;
  std::string line = *tex;
  _texWidth = line.find('\n');

  _texHeight = _texWidth;
  _textureMipMap = *tex;
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
      repeatingVerticalUpscale(height, width);
    }
    if (_texHeight > height) {
      repeatingVerticalDownscale(height, width);
    }
  } else if (mode == "interpolating") {
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
    }
  }
  GenerateTextureMask();
}

void TextureMapper::rescaleCurrentTexture(float distance) {
  setCurrentTexture(distance, "interpolating", &_textureMipMap);
}

void TextureMapper::repeatingHorizontalDownscale(int width) {
  std::string tex = "";
  tex.reserve(_texHeight * (width + 1));
  if (_texWidth > width) {
    for (int i = 0; i < _texHeight; i++) {
      tex.append(_textureMipMap.substr(i * _texWidth + i, width));
      tex.push_back(L'\n');
    }
  }
  _texWidth = width;
  _textureMipMap = std::move(tex);
}

void TextureMapper::repeatingVerticalDownscale(int height, int width) {
  _textureMipMap = _textureMipMap.substr(0, width * height + height);
  _texHeight = height;
}

void TextureMapper::repeatingHorizontalUpscale(int height, int width) {
  std::string line = "";
  line.reserve(width);
  std::string new_tex = "";
  new_tex.reserve(height * width);

  for (int i = 0; i < _texHeight; i++) {
    line = _textureMipMap.substr((1 + _texWidth) * i, _texWidth);
    while (line.length() < width)
      line.append(line.substr(0, width - line.length()));
    new_tex.append(line);
    new_tex.push_back(L'\n');
  }
  _textureMipMap = std::move(new_tex);
  _texWidth = width;
}

void TextureMapper::repeatingVerticalUpscale(int height, int width) {
  std::string new_tex = _textureMipMap;
  int i = 0;
  while (_texHeight < height) {
    new_tex += new_tex.substr((_texWidth + 1) * i, _texWidth);
    i = (i + 1) % _texHeight;
    _texHeight++;
  }
  _textureMipMap = std::move(new_tex);
}

std::string TextureMapper::getNextTexColumn(int height) {
  if (_texHeight < 1 || _texWidth < 1) {
    return std::string("");
  }
  if (_stepper >= _texWidth) {
    _stepper = 0;
  }
  std::string ret = "";
  ret.reserve(height);

  for (int i = 0; i < _texHeight; i++) {
    ret.push_back(
        _textureMipMap[(i % _texHeight) * (_texWidth + 1) + _stepper]);
  }
  int j = 0;
  while (ret.length() < height) {
    ret.push_back(
        _textureMipMap[(j % _texHeight) * (_texWidth + 1) + _stepper]);
    j++;
  }
  _stepper++;
  return ret;
}

std::vector<int> TextureMapper::getMaskColumn(int height) const {
  if (_texHeight < 1 || _texWidth < 1) {
    return std::vector<int>(0);
  }
  std::vector<int> ret;
  ret.reserve(height);

  for (int i = 0; i < _texHeight; i++) {
    ret.push_back(_textureMask[(i % _texHeight) * _texWidth + _stepper]);
  }
  int j = 0;
  while (ret.size() < height) {
    ret.push_back(_textureMask[(j % _texHeight) * _texWidth + _stepper]);
    j++;
  }
  return ret;
}

const std::string &TextureMapper::getTexture() const { return _textureMipMap; }
const std::vector<int> &TextureMapper::getMask() const { return _textureMask; }

// =======================
//  interpolation scaling
// =======================
void TextureMapper::nxInterpolationDownscale(int width) {
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
  _texWidth = width;
}

void TextureMapper::nxInterpolationUpscale(int width) {
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
  _texWidth = width;
}

void TextureMapper::nyInterpolationDownscale(int height) {
  std::string new_tex;
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
  _texHeight = height;
}

void TextureMapper::nyInterpolationUpscale(int height) {
  std::string new_tex;
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
  _texHeight = height;
}

std::string TextureMapper::ScaleToHeight(int height, std::string column) {
  std::string r_column = "";
  r_column.reserve(height);
  while (r_column.length() < height) {
    for (char c : column) {
      r_column.push_back(c);
    }
  }
  return r_column.substr(0, height);
}
