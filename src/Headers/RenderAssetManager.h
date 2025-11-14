#ifndef RENDERASSETMANAGER_H
#define RENDERASSETMANAGER_H

#include "MapManager.h"
#include "SceneManager.h"
#include <string>

class RenderAssetManager {
public:
  void rescaleTextureOf(int pos_x, int pos_y, float distance);
  std::wstring getTextureAt(int pos_x, int pos_y);
  std::wstring getNextCharColumn();

  RenderAssetManager(EntityManager &entityMan, MapManager &mapMan);

private:
  EntityManager &_entityManager;
  MapManager &_mapManager;
};
#endif // RENDERASSETMANAGER_H
