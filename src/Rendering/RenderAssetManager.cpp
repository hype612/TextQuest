#include "../Headers/RenderAssetManager.h"
#include "../Headers/Logger.h"
#include "SceneManager.h"
#include "TextureRequest.h"
#include <string>
#include <vector>

RenderAssetManager::RenderAssetManager(const SceneManager &sceneMan)
    : _mapManager(sceneMan._mapManager),
      _entityManager(sceneMan._entityManager) {}

int RenderAssetManager::getShadingIndex(float distance) const {
  auto it = std::upper_bound(_shadingThresholds.begin(),
                             _shadingThresholds.end(), distance);
  return static_cast<int>(std::distance(_shadingThresholds.begin(), it));
}

std::string RenderAssetManager::charColumn(const TextureRequest &tRequest) {
  std::string col;
  col.reserve(tRequest.height);
  if (tRequest.mapX == -1 && tRequest.mapY == -1) {
    return std::string();
  }
  if (_distanceShadingEnabled && _shadingThresholds.empty()) {
    Logger::GetInstance()->log(
        "WARNING: Distance based shading is enabled, but the thresholds are "
        "not set. Disabling distance based shading...",
        LogType::TEXPREP, LogLevel::WARNING);
    _distanceShadingEnabled = false;
  }
  int shadingIdx = getShadingIndex(tRequest.distance);
  if (_distanceShadingEnabled)
    col = _mapManager.getWallTexColumnAt(
        tRequest.mapX, tRequest.mapY, tRequest.height, tRequest.hitPoint,
        tRequest.visibleTop, tRequest.visibleBot, shadingIdx);
  else
    col = _mapManager.getWallTexColumnAt(
        tRequest.mapX, tRequest.mapY, tRequest.height, tRequest.hitPoint,
        tRequest.visibleTop, tRequest.visibleBot);

  return col;
}

std::string RenderAssetManager::scaledEntityTex(int entityId, int width,
                                                int height,
                                                float distance) const {
  return _entityManager.scaledTexOfEntity(entityId, width, height,
                                          getShadingIndex(distance));
}

void RenderAssetManager::setDistanceShading(bool enabled) {
  _distanceShadingEnabled = enabled;
}

void RenderAssetManager::setDistanceShadingThresholds(
    const std::vector<float> &thresholds) {
  _shadingThresholds = thresholds;
}
