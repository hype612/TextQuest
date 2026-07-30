#ifndef RENDERASSETMANAGER_H
#define RENDERASSETMANAGER_H

#include "EntityManager.h"
#include "MapManager.h"
#include "TextureRequest.h"
#include <string>

class SceneManager;

class RenderAssetManager {
public:
  RenderAssetManager(const SceneManager &sceneMan);

  // std::string getNextCharColumn(int height);
  std::string charColumn(const TextureRequest &tRequest);
  void setDistanceShading(bool enabled);
  void setDistanceShadingThresholds(const std::vector<float> &thresholds);
  std::string scaledEntityTex(int entityId, int width, int height,
                              float distance) const;

private:
  int getShadingIndex(float distance) const;
  const MapManager &_mapManager;
  const EntityManager &_entityManager;
  bool _distanceShadingEnabled = false;
  std::vector<float> _shadingThresholds;
};
#endif // RENDERASSETMANAGER_H
