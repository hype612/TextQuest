#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "../Headers/Entity.h"
#include "../Headers/CoordHash.h"
#include <vector>
#include <string>


class EntityManager
{
public:
  EntityManager();

  Entity& getEntityAtPos(int coord_x, int coord_y);
  int getEntityIdAtPos(int coord_x, int coord_y);
  void process();


  // ways to access the container
  void addEntity(Entity& entity);
  void removeEntity(int id);
  void removeEntity(int coord_x, int coord_y);
  void removeAllEntities();


  // for renderer
  void rescaleEntityTexture(int EntityId, float distance);
  void changeEntityTextureScale(Entity& entity, float distance);
  std::wstring getCurrentEntityTexture(int EntityId);
  std::wstring getCurrentEntityTexture(const Entity& entity);
  std::wstring getCurrentEntityTexMask(int EntityId);
  std::wstring getCurrentEntityTexMask(const Entity& entity);

  std::wstring getNextEntityCharColumn(int EntityId);
  std::wstring getEntityMaskColumn(int EntityId);

private:
  std::vector<Entity> _entityContainer;
}


#endif // ENTITYMANAGER_H
