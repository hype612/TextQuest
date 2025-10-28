#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "../Headers/Entity.h"
#include "../Headers/CoordHash.h"
#include <vector>

class EntityManager
{
public:
  EntityManager();

  Entity getEntityAtPos(int coord_x, int coord_y);
  int getEntityIdAtPos(int coord_x, int_coord_y);
  // later add possiblity to request entityState
  
  void ScaleEntityTexture(int EntityId);
  void GetEntityTexture(int EntityId);

  void addEntity(Entity& entity);
  void removeEntity(int id);
  void removeEntity(int coord_x, int coord_y);

private:
  std::vector<Entity> _entityContainer;  
}


#endif // ENTITYMANAGER_H
