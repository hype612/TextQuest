#ifndef TEXTUREMAPPER_H
#define TEXTUREMAPPER_H

#include "./Logger.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class TextureMapper {
public:
  float estimateHeight(float distance);
  float estimateWidth(float distance);
  void setCurrentTexture(float distance, const std::string &mode,
                         std::string *tex);
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

  void nxyInterpolationScale(int width, int height);
  void nxInterpolationDownscale(int width);
  void nxInterpolationUpscale(int width);
  void nyInterpolationDownscale(int height);
  void nyInterpolationUpscale(int height);

  // =========================
  //   return of textures
  // =========================
  std::string getTextColumnAt(int height, float hitpoint);
  std::vector<int> getMaskColumnAt(int height, float hitpoint);
  // std::string getNextTexColumn(int height);
  // std::vector<int> getMaskColumn(int height) const;
  const std::string &getTexture() const;
  const std::vector<int> &getMask() const;

  TextureMapper(std::string initTexture);
  TextureMapper() = default;

private:
  std::string _textureMipMap = "";
  std::vector<int> _textureMask;
  int _stepper = -1;
  int _texHeight = -1;
  int _texWidth = -1;

  void GenerateTextureMask();
  std::string ScaleToHeight(int height, std::string column);
  char sampleNN(float u, float v);
};

#endif
