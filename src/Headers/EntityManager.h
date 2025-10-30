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


  void ScaleEntityTexture(int EntityId);
  void GetEntityTexture(int EntityId);

  // ways to access the container
  void addEntity(Entity& entity);
  void removeEntity(int id);
  void removeEntity(int coord_x, int coord_y);

  // for renderer
  void ChangeEntityTextureScale(int EntityId, float distance);
  void ChangeEntityTextureScale(Entity& entity, float distance);
  std::wstring GetCurrentEntityTexture(int EntityId);
  std::wstring GetCurrentEntityTexture(const Entity& entity);

private:
  std::vector<Entity> _entityContainer;
}


#endif // ENTITYMANAGER_H
