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

  void repeatingVerticalDownscale(unsigned int height, unsigned int width);
  void repeatingHorizontalDownscale(unsigned int width);
  void repeatingHorizontalUpscale(unsigned int height, unsigned int width);
  void repeatingVerticalUpscale(unsigned int height);

  // =============================
  //   interpolation scaling(nn)
  // =============================

  void nxyInterpolationScale(unsigned int width, unsigned int height);
  void nxInterpolationDownscale(unsigned int width);
  void nxInterpolationUpscale(unsigned int width);
  void nyInterpolationDownscale(unsigned int height);
  void nyInterpolationUpscale(unsigned int height);

  // =========================
  //   return of textures
  // =========================
  std::string getTextColumnAt(unsigned int height, float hitpoint);
  std::vector<int> getMaskColumnAt(unsigned int height, float hitpoint);
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
  unsigned int _texHeight = -1;
  unsigned int _texWidth = -1;

  void GenerateTextureMask();
  std::string ScaleToHeight(unsigned int height, std::string column);
  char sampleNN(float u, float v);
};

#endif
