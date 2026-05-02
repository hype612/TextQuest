#include "../Headers/RenderAssetManager.h"

// UPDATE: Rescaling is unneccessary and just takes up processing time
// do the scaling at fetch. This class will be remodeled to
// handle the texture-combing of different layers probably

void RenderAssetManager::TexturePreparator() {
  // fetch col
  while (!_texRequestQ.isEmpty()) {
  }
}

void RenderAssetManager::prepareEntityTexture(
    const std::tuple<int, int, Tile, float> &toPrepare) {
  int id = _entityManager.getEntityIdAtPos(std::get<0>(toPrepare),
                                           std::get<1>(toPrepare));
  //_entityManager.rescaleEntityTexture(id, std::get<3>(toPrepare));
}

void RenderAssetManager::prepareWallTexture(
    const std::tuple<int, int, Tile, float> &toPrepare) {
  //_mapManager.rescaleWallTextureAt(
  //    std::get<0>(toPrepare), std::get<1>(toPrepare), std::get<3>(toPrepare));
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
  std::string col;
  while (!_texRequestQ.isEmpty()) {
    TextureRequest t = _texRequestQ.pop();
  }
  /*
  if (_depthStack.empty()) {
    col = std::string(height, ' ');
    return col;
  }
  int x = std::get<0>(_depthStack.back());
  int y = std::get<1>(_depthStack.back());
  if (std::get<2>(_depthStack.back()) == Tile::WALL) {
    col.append(_mapManager.getWallTexColumnAt(x, y, height));
    _depthStack.pop_back();
  }
  while (!_depthStack.empty())
    _depthStack.pop_back();*/
  return col;
}
