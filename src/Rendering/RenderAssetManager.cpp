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
  if (_mapManager.isWall(pos_x, pos_y) == true) {
    return _mapManager.getWallTextureAt(pos_x, pos_y);
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
  Logger::GetInstance()->log("getNextCharColumn called, queue empty=" +
                                 std::to_string(_texRequestQ.isEmpty()),
                             LogType::RENDER, LogLevel::INFO);
  Logger::GetInstance()->forceFlush();
  std::string col;
  col.reserve(height);
  while (!_texRequestQ.isEmpty()) {
    TextureRequest t = _texRequestQ.pop();
    if (t.mapX == -1 && t.mapY == -1 && t.tileType == Tile::NONE) {
      break;
    }
    if (t.tileType == Tile::WALL) {
      if (_distanceShadingEnabled)
        col = _mapManager.getWallTexColumnAt(t.mapX, t.mapY, t.height,
                                             t.hitPoint, t.distance);
      else
        col = _mapManager.getWallTexColumnAt(t.mapX, t.mapY, t.height,
                                             t.hitPoint);
    } else if (t.tileType == Tile::ENTITY) {
      std::vector<int> txMask = _entityManager.getEntityMaskColAt(
          t.mapX, t.mapY, t.height, t.hitPoint);
      std::string tx;
      if (_distanceShadingEnabled)
        tx = _entityManager.getEntityTexColAt(t.mapX, t.mapY, t.height,
                                              t.hitPoint, t.distance);
      else
        tx = _entityManager.getEntityTexColAt(t.mapX, t.mapY, t.height,
                                              t.hitPoint);
      for (int i = 0; i < t.height; i++) {
        if (txMask[i] == 1)
          col[i] = tx[i];
      }
    }
  }

  return col;
}

void RenderAssetManager::setDistanceShading(bool enabled) {
  _distanceShadingEnabled = enabled;
}
