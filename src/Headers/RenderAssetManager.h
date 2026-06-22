#ifndef RENDERASSETMANAGER_H
#define RENDERASSETMANAGER_H

#include "EngineState.h"
#include "MapManager.h"
#include "SceneManager.h"
#include "TextureRequest.h"
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
  std::string getTextureAt(int pos_x, int pos_y);
  std::string getNextCharColumn(int height);
  void setDistanceShading(bool enabled);

private:
  EntityManager &_entityManager;
  MapManager &_mapManager;
  TextureRequestQueue &_texRequestQ;
  bool _distanceShadingEnabled = false;
};
#endif // RENDERASSETMANAGER_H
