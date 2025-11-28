#include "../Headers/RenderAssetManager.h"

void RenderAssetManager::TexturePreparator() {
  while (EngineState::GetInstance()->gameRunningf == true) {
    std::tuple<int, int, Tile, float> current = _texRequestQ.pop();

    if (std::get<0>(current) == -1 && std::get<1>(current) == -1 &&
        _texRequestQ.isRayCompleted()) {
      _texRequestQ.setTexturesReady(true);
      continue;
    }

    switch (std::get<2>(current)) {
    case Tile::ENTITY:
      prepareEntityTexture(current);
      break;
    case Tile::WALL:
      prepareWallTexture(current);
      break;
    default:
      continue;
    }
    _depthStack.push_back(std::tuple<int, int, Tile>(
        std::get<0>(current), std::get<1>(current), std::get<2>(current)));
  }
}

void RenderAssetManager::prepareEntityTexture(
    const std::tuple<int, int, Tile, float> &toPrepare) {
  // TODO: make it available to have multiple entities on a
  // single tile. For that, entity texture preparation needs to work with arrays
  int id = _entityManager.getEntityIdAtPos(std::get<0>(toPrepare),
                                           std::get<1>(toPrepare));
  _entityManager.rescaleEntityTexture(id, std::get<3>(toPrepare));
}

void RenderAssetManager::prepareWallTexture(
    const std::tuple<int, int, Tile, float> &toPrepare) {
  _mapManager.rescaleWallTextureAt(
      std::get<0>(toPrepare), std::get<1>(toPrepare), std::get<3>(toPrepare));
}

RenderAssetManager::RenderAssetManager(EntityManager &entityMan,
                                       MapManager &mapMan,
                                       TextureRequestQueue &texReqQ)
    : _entityManager(entityMan), _mapManager(mapMan), _texRequestQ(texReqQ) {}

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

std::wstring RenderAssetManager::getNextCharColumn(int height) {
  std::wstring col;
  while (!_depthStack.empty()) {
    std::wstring thisCol;
    int x = std::get<0>(_depthStack.back());
    int y = std::get<1>(_depthStack.back());
    if (std::get<2>(_depthStack.back()) == Tile::WALL) {
      col.append(_mapManager.getWallTexColumnAt(x, y, height));
      continue;
    }
    int id = _entityManager.getEntityIdAtPos(x, y);
    if (col.empty()) {
      col.append(_entityManager.getNextEntityCharColumn(id, height));
      continue;
    }
    thisCol = _entityManager.getNextEntityCharColumn(id, height);
    bool edge_detected = false;
    int i = col.size();
    for (auto it = thisCol.rbegin(); it != thisCol.rend(); ++it) {
      if (i < 0) {
        break;
      }
      auto &c = *it;
      if (c == L' ' && edge_detected == false) {
        continue;
      }
      edge_detected = !edge_detected;
      if (c != L' ')
        col[i] = c;
      i--;
    }
    _depthStack.pop_back();
  }
  return col;
}
