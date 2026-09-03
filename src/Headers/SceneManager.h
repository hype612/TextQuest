#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Camera.h"
#include "Entity.h"
#include "EntityDistance.h"
#include "EntityManager.h"
#include "IEntitySceneChannel.h"
#include "MapManager.h"
#include "RenderAssetManager.h"
#include "Transform.h"
#include <string>
#include <unordered_map>
#include <vector>

class SceneManager : public IEntitySceneChannel {
public:
  friend RenderAssetManager::RenderAssetManager(const SceneManager &sceneMan);

  SceneManager();

  void process(float delta);

  // Map Related functions
  void initializeNewMap(std::string &map, int mapWidth, int mapHeight);
  int getMapHeight() const;
  int getMapWidth() const;
  bool isMapAvailable() const;

  void uploadTextureForWall(const char &mapChar, std::string &wallTex);
  void uploadTextureVecForWall(const char &mapChar,
                               std::vector<std::string> &wallTexV);

  // Channel implementation
  bool canMoveTo(const vec2f &dest) const override;
  bool isOutOfBounds(int test_x, int test_y) const;
  bool isWall(int test_x, int test_y) const;

  // Entity Related functions
  Entity &entityAtId(int id);
  void AddEntity(Entity &entity);
  void removeEntity(Entity &entity);
  void removeEntity(int entityId);
  void removeAllEntities();
  std::vector<EntityDistance>
  entitiesSortedByDistanceTo(const vec2f &target) const;

  // Camera
  const Camera &camera() const;
  const Camera *cameraPtr() const;
  const Transform &cameraFollow() const;
  void setCameraFollow(const Transform &new_follow);
  void setCameraFovDegrees(float newfov);

  // Other
  void loadResources(const std::string &filePath,
                     std::unordered_map<std::string, std::string> &outTextures);

private:
  EntityManager _entityManager;
  MapManager _mapManager;
  // need to postpone construction
  std::optional<Camera> _camera;
};

#endif // SCENEMANAGER_H
