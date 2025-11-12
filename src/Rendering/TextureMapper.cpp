#include "../Headers/TextureMapper.h"

TextureMapper::TextureMapper(std::wstring initTexture)
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
  int tx_width = _textureMipMap.find(L'\n');
  int tx_height = tx_width;
  std::vector<int> ret(tx_height, tx_width);
  for (int i = 0; i < tx_height; i++) {
    for (int j = 0; j < tx_width; j++) {
      if (_textureMipMap[i * tx_width + i + j] == ' ')
        ret[i, j] = 0;
      else
        ret[i, j] = 1;
    }
  }
}

void TextureMapper::setCurrentTexture(float distance, const std::string &mode,
                                      std::wstring *tex) {
  _stepper = 0;
  int height = (int)(estimateHeight(distance));
  int width = (int)(estimateWidth(distance));

  std::wstring line = *tex;
  _texWidth = line.find(L'\n');

  _texHeight = _texWidth;
  _textureMipMap = *tex;
  // HORIZONTAL SCALE
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
  GenerateTextureMask();
}

void TextureMapper::rescaleCurrentTexture(float distance) {
  setCurrentTexture(distance, "interpolating", &_textureMipMap);
}

void TextureMapper::repeatingHorizontalDownscale(int height, int width) {
  std::wstring tex = L"";
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
  std::wstring line = L"";
  line.reserve(width);
  std::wstring new_tex = L"";
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
  std::wstring new_tex = _textureMipMap;
  int i = 0;
  while (_texHeight < height) {
    new_tex += new_tex.substr((_texWidth + 1) * i, _texWidth);
    i = (i + 1) % _texHeight;
    _texHeight++;
  }
  _textureMipMap = std::move(new_tex);
}

std::wstring TextureMapper::getNextTexColumn(int height) {
  if (_texHeight < 1 || _texWidth < 1) {
    return std::wstring(L"");
  }
  if (_stepper >= _texWidth) {
    _stepper = 0;
  }
  std::wstring ret = L"";
  ret.reserve(height);

  for (int i = 0; i < _texHeight; i++) {
    ret.push_back(_textureMipMap[(i % _texHeight) * _texWidth + _stepper]);
  }
  int j = 0;
  while (ret.length() < height) {
    ret.push_back(_textureMipMap[(j % _texHeight) * _texWidth + _stepper]);
    j++;
  }
  _stepper++;
  return ret;
}

std::wstring TextureMapper::getMaskColumn(int height) const {
  if (_texHeight < 1 || _texWidth < 1) {
    return std::wstring(L"");
  }
  std::wstring ret = L"";
  ret.reserve(height);

  for (int i = 0; i < _texHeight; i++) {
    ret.push_back(_textureMask[(i % _texHeight) * _texWidth + _stepper]);
  }
  int j = 0;
  while (ret.length() < height) {
    ret.push_back(_textureMask[(j % _texHeight) * _texWidth + _stepper]);
    j++;
  }
  return ret;
}

std::wstring TextureMapper::getTexture() const { return _textureMipMap; }
std::wstring TextureMapper::getMask() const { return _textureMask; }

// =======================
//  interpolation scaling
// =======================
void TextureMapper::nxInterpolationDownscale(int width) {
  std::wstring new_tex;
  new_tex.reserve(width * _texHeight);

  float x_ratio = static_cast<float>(_texWidth) / width;

  for (int y = 0; y < _texHeight; y++) {
    for (int x = 0; x < width; x++) {
      int src_x = static_cast<int>(x * x_ratio);
      wchar_t pb = _textureMipMap[y * _texWidth + src_x + y];
      if (pb != L'\n')
        new_tex.push_back(pb);
      else
        new_tex.push_back(_textureMipMap[y * _texWidth + src_x - 1]);
    }
    new_tex.push_back(L'\n');
  }
  _textureMipMap = std::move(new_tex);
  _texWidth = width;
}

void TextureMapper::nxInterpolationUpscale(int width) {
  std::wstring new_tex;
  new_tex.reserve(width * _texHeight);

  float x_ratio = static_cast<float>(_texWidth) / width;

  for (int y = 0; y < _texHeight; y++) {
    for (int x = 0; x < width; x++) {
      int src_x = static_cast<int>(x * x_ratio);
      wchar_t pb = _textureMipMap[y * _texWidth + src_x + y];
      if (pb != L'\n')
        new_tex.push_back(pb);
      else
        new_tex.push_back(_textureMipMap[y * _texWidth + src_x - 1]);
    }
    new_tex.push_back(L'\n');
  }
  _textureMipMap = std::move(new_tex);
  _texWidth = width;
}

void TextureMapper::nyInterpolationDownscale(int height) {
  std::wstring new_tex;
  new_tex.reserve(_texWidth * height);

  float y_ratio = static_cast<float>(_texHeight) / height;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < _texWidth; x++) {
      int src_y = static_cast<int>(y * y_ratio);
      wchar_t pb = _textureMipMap[src_y * _texWidth + x + src_y];
      if (pb != L'\n')
        new_tex.push_back(pb);
      else
        new_tex.push_back(_textureMipMap[src_y * _texWidth + x - 1]);
    }
    new_tex.push_back(L'\n');
  }
  _textureMipMap = std::move(new_tex);
  _texHeight = height;
}

void TextureMapper::nyInterpolationUpscale(int height) {
  std::wstring new_tex;
  new_tex.reserve(_texWidth * height);

  float y_ratio = static_cast<float>(_texHeight) / height;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < _texWidth; x++) {
      int src_y = static_cast<int>(y * y_ratio);
      wchar_t pb = _textureMipMap[src_y * _texWidth + x + src_y];
      if (pb != L'\n')
        new_tex.push_back(pb);
      else
        new_tex.push_back(_textureMipMap[src_y * _texWidth + x - 1]);
    }
    new_tex.push_back(L'\n');
  }
  _textureMipMap = std::move(new_tex);
  _texHeight = height;
}

std::wstring TextureMapper::ScaleToHeight(int height, std::wstring column) {
  std::wstring r_column = L"";
  r_column.reserve(height);
  while (r_column.length() < height) {
    for (wchar_t c : column) {
      r_column.push_back(c);
    }
  }
  return r_column.substr(0, height);
}
