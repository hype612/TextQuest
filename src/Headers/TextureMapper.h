#ifndef TEXTUREMAPPER_H
#define TEXTUREMAPPER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class TextureMapper {
public:
  float estimateHeight(float distance);
  float estimateWidth(float distance);
  void setCurrentTexture(float distance, const std::string &mode,
                         std::wstring *tex);
  void rescaleCurrentTexture(float distance);
  // =============================
  // repeating scaling
  // =============================

  void repeatingVerticalDownscale(int height, int width);
  void repeatingHorizontalDownscale(int width);
  void repeatingHorizontalUpscale(int height, int width);
  void repeatingVerticalUpscale(int height, int width);

  // =============================
  //   interpolation scaling(nn)
  // =============================

  void nxInterpolationDownscale(int width);
  void nxInterpolationUpscale(int width);
  void nyInterpolationDownscale(int height);
  void nyInterpolationUpscale(int height);

  // =========================
  //   return of textures
  // =========================
  std::wstring getNextTexColumn(int height);
  std::vector<int> getMaskColumn(int height) const;
  const std::wstring &getTexture() const;
  const std::vector<int> &getMask() const;

  TextureMapper(std::wstring initTexture);
  TextureMapper() = default;

private:
  std::wstring _textureMipMap = L"";
  std::vector<int> _textureMask;
  int _stepper = -1;
  int _texHeight = -1;
  int _texWidth = -1;

  void GenerateTextureMask();
  std::wstring ScaleToHeight(int height, std::wstring column);
};

#endif
