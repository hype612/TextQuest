#include "../Headers/RenderAssetManager.h"
#include <string>
#include <vector>

void RenderAssetManager::TexturePreparator() {
  while (!_texRequestQ.isEmpty()) {
  }
}

RenderAssetManager::RenderAssetManager(EntityManager &entityMan,
                                       MapManager &mapMan,
                                       TextureRequestQueue &texReqQ)
    : _entityManager(entityMan), _mapManager(mapMan), _texRequestQ(texReqQ) {}

std::string RenderAssetManager::getTextureAt(int pos_x, int pos_y) {
  if (_mapManager.isWall(pos_x, pos_y)) {
    // return _mapManager.getWallTextureAt(pos_x, pos_y);
  }
  int e_id = _entityManager.getEntityIdAtPos(pos_x, pos_y);
  if (e_id != -1) {
    return _entityManager.getCurrentEntityTexture(e_id);
  } else {
    std::cerr << "invalid position" << std::endl;
    return std::string();
  }
}

std::string RenderAssetManager::getNextCharColumn(int height) {
  std::string col;
  col.reserve(height);
  while (!_texRequestQ.isEmpty()) {
    TextureRequest t = _texRequestQ.pop();
    if (t.mapX == -1 && t.mapY == -1) {
      break;
    }
    if (_distanceShadingEnabled && _shadingThresholds.empty()) {
      Logger::GetInstance()->log(
          "WARNING: Distance based shading is enabled, but the thresholds are "
          "not set. Disabling distance based shading...",
          LogType::TEXPREP, LogLevel::WARNING);
      _distanceShadingEnabled = false;
    }
    int shadingIdx = (int)t.distance;
    if (_distanceShadingEnabled)
      col = _mapManager.getWallTexColumnAt(t.mapX, t.mapY, t.height, t.hitPoint,
                                           t.wallTop, shadingIdx);
    else
      col = _mapManager.getWallTexColumnAt(t.mapX, t.mapY, t.height, t.hitPoint,
                                           t.wallTop);
  }

  return col;
}

void RenderAssetManager::setDistanceShading(bool enabled) {
  _distanceShadingEnabled = enabled;
}

void RenderAssetManager::setDistanceShadingThresholds(
    const std::vector<float> &thresholds) {
  _shadingThresholds = thresholds;
}
