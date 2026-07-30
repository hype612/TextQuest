#ifndef TEXTUREMAPPER_H
#define TEXTUREMAPPER_H

#include <string>
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

  std::string nxyInterpolationScale(unsigned int width, unsigned int height,
                                    int shadingIdx) const;

  // =========================
  //   return of textures
  // =========================
  std::string getTexColumnAt(unsigned int height, float hitpoint,
                             int visibleTop, int visibleBot) const;
  std::string getTexColumnAt(unsigned int height, float hitpoint,
                             int visibleTop, int visibleBot,
                             int shadingIdx) const;
  std::string scaledTex(unsigned int width, unsigned int height,
                        int shadingIdx) const;
  std::vector<int> scaledMask(unsigned int width, unsigned int height) const;
  std::vector<int> getMaskColumnAt(unsigned int height, float hitpoint) const;
  const std::string &getTexture() const;
  const std::vector<int> &getMask() const;

  TextureMapper(std::string initTexture);
  // Expects all textures of the vec to be the same size
  // and vec[0] == brightest => vec[size] dimmest
  TextureMapper(std::vector<std::string> initTexture);
  TextureMapper() = default;

private:
  std::vector<std::string> _textureMipMaps;
  std::vector<int> _textureMask;
  unsigned int _texHeight = -1;
  unsigned int _texWidth = -1;

  void GenerateTextureMask();
  char sampleNN(float u, float v, int shadingIdx) const;
  int samepleNNonMask(float u, float v) const;
};

#endif
