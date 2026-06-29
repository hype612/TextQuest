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
  int getEntityIdAtPos(int coordX, int coordY) const;
  int getEntityCount() const;
  void process();

  // ways to access the container
  void addEntity(Entity &entity);
  void removeEntity(int id);
  void removeEntity(int coord_x, int coord_y);
  void removeAllEntities();

  // for renderer
  std::string getCurrentEntityTexture(int EntityId);
  std::string getCurrentEntityTexture(const Entity &entity);
  const std::vector<int> &getCurrentEntityTexMask(int EntityId);
  const std::vector<int> &getCurrentEntityTexMask(const Entity &entity);

  std::string getEntityTexColAt(int coordX, int coordY, int height,
                                float hitpoint, int wallTop);
  std::string getEntityTexColAt(int coordX, int coordY, int height,
                                float hitpoint, int wallTop, int shadingidx);
  std::vector<int> getEntityMaskColAt(int coordX, int coordY, int height,
                                      float hitpoint);

private:
  std::vector<Entity> _entityContainer;
};

#endif // ENTITYMANAGER_H
