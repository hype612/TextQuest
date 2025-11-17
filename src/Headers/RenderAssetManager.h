#ifndef RENDERASSETMANAGER_H
#define RENDERASSETMANAGER_H

#include "EngineState.h"
#include "MapManager.h"
#include "SceneManager.h"
#include "TextureRequestQueue.h"
#include <string>

class RenderAssetManager {
public:
  RenderAssetManager(EntityManager &entityMan, MapManager &mapMan,
                     TextureRequestQueue &texReqQ);

  void TexturePreparator();
  void rescaleTextureOf(int pos_x, int pos_y, float distance);
  std::wstring getTextureAt(int pos_x, int pos_y);
  std::wstring getNextCharColumn();

private:
  EntityManager &_entityManager;
  MapManager &_mapManager;
  TextureRequestQueue &_texRequestQ;
};
#endif // RENDERASSETMANAGER_H
