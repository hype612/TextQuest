#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "../Headers/Entity.h"
#include "../Headers/EntityDistance.h"
#include "../Headers/IEntitySceneChannel.h"
#include <string>
#include <vector>

class EntityManager {
public:
  EntityManager(IEntitySceneChannel &channel);
  void process(float delta);

  // ================================
  // ways to access items in container
  // ================================

  int getEntityIdAtPos(int coordX, int coordY) const;
  int getEntityCount() const;

  // TODO: rework these so the ids are persistant.
  // possibly implement swap-pop to save on time

  void addEntity(Entity &entity);
  void removeEntity(int id);
  void removeAllEntities();
  Entity &entityAtId(int id);

  // ================================
  // for renderer
  // ================================

  std::string getCurrentEntityTexture(int EntityId) const;
  std::string getCurrentEntityTexture(const Entity &entity) const;
  const std::vector<int> &getCurrentEntityTexMask(int EntityId) const;
  const std::vector<int> &getCurrentEntityTexMask(const Entity &entity) const;

  std::string scaledTexOfEntity(int entityId, unsigned int width,
                                unsigned int height, int shadingIdx) const;
  std::vector<int> scaledMaskOfEntity(int entityId, unsigned int width,
                                      unsigned int height) const;
  // NOT descending
  std::vector<EntityDistance>
  entitiesSortedByDistanceTo(const vec2f &target) const;

private:
  void resolveStates();
  void resolveProjectiles();
  void resolveMovement(float delta);
  void resolveVisibility();

  std::optional<float> projectileHitDistSq(vec2f origin, vec2f target,
                                           float hit_delta, vec2f dir,
                                           float max_dist);
  std::vector<Entity> _entityContainer;
  IEntitySceneChannel &_sceneChannel;
};

#endif // ENTITYMANAGER_H
