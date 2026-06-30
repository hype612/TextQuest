#ifndef TEXTUREMAPPER_H
#define TEXTUREMAPPER_H

#include "./Logger.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class TextureMapper {
public:
  // For the implementation of distance based shading
  // everything that has been unused for a while been treated as
  // only working for distance based shading turned off
  // TODO: implement repeating scaling for distance based shading

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

  void nxyInterpolationScale(unsigned int width, unsigned int height,
                             float shadingIdx);

  // =========================
  //   return of textures
  // =========================
  std::string getTexColumnAt(unsigned int height, float hitpoint,
                             int wallTop) const;
  std::string getTexColumnAt(unsigned int height, float hitpoint, int wallTop,
                             int shadingIdx) const;
  std::vector<int> getMaskColumnAt(unsigned int height, float hitpoint) const;
  const std::string &getTexture() const;
  const std::vector<int> &getMask() const;

  TextureMapper(std::string initTexture);
  TextureMapper(std::vector<std::string> initTexture);
  TextureMapper() = default;

private:
  std::vector<std::string> _textureMipMaps;
  std::vector<int> _textureMask;
  unsigned int _texHeight = -1;
  unsigned int _texWidth = -1;

  void GenerateTextureMask();
  char sampleNN(float u, float v, int shadingIdx) const;
};

#endif
