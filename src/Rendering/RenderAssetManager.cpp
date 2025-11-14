#include "../Headers/RenderAssetManager.h"

RenderAssetManager::RenderAssetManager(EntityManager &entityMan,
                                       MapManager &mapMan)
    : _entityManager(entityMan), _mapManager(mapMan) {}

void RenderAssetManager::rescaleTextureOf(int pos_x, int pos_y,
                                          float distance) {
  if (_mapManager.isWall(pos_x, pos_y) == true)
    _mapManager.rescaleWallTextureAt(pos_x, pos_y);
  int e_id = _entityManager.getEntityIdAtPos(pos_x, pos_y);
  if (e_id != -1)
    _entityManager.rescaleEntityTexture(e_id, distance);
  else
    std::cerr << "invalid position" << std::endl;
}

std::wstring RenderAssetManager::getTextureAt(int pos_x, int pos_y) {
  if (_mapManager.isWall() == true)
    _mapManager.getWallTextureAt(pos_x, pos_y);
  int e_id = _entityManager.getEntityIdAtPos(pos_x, pos_y);
  if (e_id != -1)
    _entityManager.getCurrentEntityTexture(e_id);
  else
    std::cerr << "invalid position" << std::endl;
}

std::wstring RenderAssetManager::getNextCharColumnAt(int pos_x, int pos_y) {
  if (_mapManager.isWall() == true)
    _mapManager.getWallTexColumnAt(pos_x, pos_y);
  int e_id = _entityManager.getEntityIdAtPos(pos_x, pos_y);
  if (e_id != -1)
    _entityManager.getNextEntityCharColumn(e_id);
  else
    std::cerr << "invalid position" << std::endl;
}
