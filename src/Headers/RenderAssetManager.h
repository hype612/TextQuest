#ifndef RENDERASSETMANAGER_H
#define RENDERASSETMANAGER_H

#include "EntityManager.h"
#include "MapManager.h"
#include "SceneManager.h"
#include "TextureRequestQueue.h"
#include <cinttypes>
#include <string>

class RenderAssetManager {
public:
  RenderAssetManager(SceneManager &sceneMan, TextureRequestQueue &texReqQ);

  void TexturePreparator();
  std::string getTextureAt(int pos_x, int pos_y);
  std::string getNextCharColumn(int height);
  void setDistanceShading(bool enabled);
  void setDistanceShadingThresholds(const std::vector<float> &thresholds);

private:
  int getShadingIndex(float distance) const;
  SceneManager &_sceneMan;
  TextureRequestQueue &_texRequestQ;
  bool _distanceShadingEnabled = false;
  std::vector<float> _shadingThresholds;
};
#endif // RENDERASSETMANAGER_H
