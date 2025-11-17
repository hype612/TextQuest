#include "../Headers/RenderAssetManager.h"

void RenderAssetManager::TexturePreparator() {
  while (EngineState::GetInstance()->GameRunningf == true) {
    if (_texRequestQ.isEmpty()) {
      continue;
    }
  }
}

RenderAssetManager::RenderAssetManager(EntityManager &entityMan,
                                       MapManager &mapMan,
                                       TextureRequestQueue &texReqQ)
    : _entityManager(entityMan), _mapManager(mapMan), _texRequestQ(texReqQ) {}

void RenderAssetManager::rescaleTextureOf(int pos_x, int pos_y,
                                          float distance) {
  if (_mapManager.isWall(pos_x, pos_y) == true)
    _mapManager.rescaleWallTextureAt(pos_x, pos_y, distance);
  int e_id = _entityManager.getEntityIdAtPos(pos_x, pos_y);
  if (e_id != -1)
    _entityManager.rescaleEntityTexture(e_id, distance);
  else
    std::cerr << "invalid position" << std::endl;
}

std::wstring RenderAssetManager::getTextureAt(int pos_x, int pos_y) {
  if (_mapManager.isWall(pos_x, pos_y) == true) {
    return _mapManager.getWallTextureAt(pos_x, pos_y);
  }
  int e_id = _entityManager.getEntityIdAtPos(pos_x, pos_y);
  if (e_id != -1) {
    return _entityManager.getCurrentEntityTexture(e_id);
  } else {
    std::cerr << "invalid position" << std::endl;
    return std::wstring();
  }
}

std::wstring RenderAssetManager::getNextCharColumn() {}
