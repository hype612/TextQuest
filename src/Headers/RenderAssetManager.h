#ifndef RENDERASSETMANAGER_H
#define RENDERASSETMANAGER_H

#include "EngineState.h"
#include "MapManager.h"
#include "SceneManager.h"
#include "TextureRequestQueue.h"
#include "Tile.h"
#include <deque>
#include <string>
#include <tuple>

class RenderAssetManager {
public:
  RenderAssetManager(EntityManager &entityMan, MapManager &mapMan,
                     TextureRequestQueue &texReqQ);

  void TexturePreparator();
  std::wstring getTextureAt(int pos_x, int pos_y);
  std::wstring getNextCharColumn(int height);

private:
  void prepareEntityTexture(const std::tuple<int, int, Tile, float> &toPrepare);
  void prepareWallTexture(const std::tuple<int, int, Tile, float> &toPrepare);

  std::deque<std::tuple<int, int, Tile>> _depthStack;
  EntityManager &_entityManager;
  MapManager &_mapManager;
  TextureRequestQueue &_texRequestQ;
};
#endif // RENDERASSETMANAGER_H
