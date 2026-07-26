#include "../Headers/RenderAssetManager.h"
#include "../Headers/Logger.h"
#include "SceneManager.h"
#include "TextureRequest.h"
#include <iostream>
#include <string>
#include <vector>

void RenderAssetManager::TexturePreparator() {
  while (!_texRequestQ.isEmpty()) {
  }
}

RenderAssetManager::RenderAssetManager(SceneManager &sceneMan,
                                       TextureRequestQueue &texReqQ)
    : _sceneMan(sceneMan), _texRequestQ(texReqQ) {}

std::string RenderAssetManager::getTextureAt(int pos_x, int pos_y) {
  /*
  if (_sceneMan.isWall(pos_x, pos_y)) {
    // return _mapManager.getWallTextureAt(pos_x, pos_y);
  }
  int e_id = _sceneMan.getEntityIdAtPos(pos_x, pos_y);
  if (e_id != -1) {
    return _entityManager.getCurrentEntityTexture(e_id);
  } else {
    std::cerr << "invalid position" << std::endl;
    return std::string();
  }
  */
  return std::string();
}
int RenderAssetManager::getShadingIndex(float distance) const {
  auto it = std::upper_bound(_shadingThresholds.begin(),
                             _shadingThresholds.end(), distance);
  return static_cast<int>(std::distance(_shadingThresholds.begin(), it));
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
    int shadingIdx = getShadingIndex(t.distance);
    Logger::GetInstance()->log("texrequest: ", LogType::TEXPREP,
                               LogLevel::INFO);
    Logger::GetInstance()->log("x:" + std::to_string(t.mapX) +
                                   " y:" + std::to_string(t.mapY) +
                                   " height:" + std::to_string(t.height) +
                                   " dist:" + std::to_string(t.distance),
                               LogType::TEXPREP, LogLevel::INFO);
    if (_distanceShadingEnabled)
      col =
          _sceneMan.getWallTexColumnAt(t.mapX, t.mapY, t.height, t.hitPoint,
                                       t.visibleTop, t.visibleBot, shadingIdx);
    else
      col = _sceneMan.getWallTexColumnAt(t.mapX, t.mapY, t.height, t.hitPoint,
                                         t.visibleTop, t.visibleBot);
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
