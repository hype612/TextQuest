#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "../Headers/CoordHash.h"
#include "../Headers/Entity.h"
#include <functional>
#include <optional>
#include <string>
#include <vector>

class EntityManager {
public:
  EntityManager();

  std::optional<std::reference_wrapper<Entity>> getEntityAtPos(int coord_x,
                                                               int coord_y);
  int getEntityIdAtPos(int coord_x, int coord_y) const;
  void process();

  // ways to access the container
  void addEntity(Entity &entity);
  void removeEntity(int id);
  void removeEntity(int coord_x, int coord_y);
  void removeAllEntities();

  // for renderer
  void rescaleEntityTexture(int EntityId, float distance);
  void changeEntityTextureScale(Entity &entity, float distance);
  std::wstring getCurrentEntityTexture(int EntityId);
  std::wstring getCurrentEntityTexture(const Entity &entity);
  std::wstring getCurrentEntityTexMask(int EntityId);
  std::wstring getCurrentEntityTexMask(const Entity &entity);

  std::wstring getNextEntityCharColumn(int EntityId, int height);
  std::wstring getEntityMaskColumn(int EntityId, int height);

private:
  std::vector<Entity> _entityContainer;
};

#endif // ENTITYMANAGER_H
