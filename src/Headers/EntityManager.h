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
  void process();

  // ================================
  // ways to access items in container
  // ================================

  std::optional<std::reference_wrapper<Entity>> getEntityAtPos(int coord_x,
                                                               int coord_y);
  int getEntityIdAtPos(int coordX, int coordY) const;
  int getEntityCount() const;

  // ================================
  // ways to modulate container
  // ================================

  void addEntity(Entity &entity);
  void removeEntity(int id);
  void removeEntity(int coord_x, int coord_y);
  void removeAllEntities();

  // ================================
  // for renderer
  // ================================

  std::string getCurrentEntityTexture(int EntityId) const;
  std::string getCurrentEntityTexture(const Entity &entity) const;
  const std::vector<int> &getCurrentEntityTexMask(int EntityId) const;
  const std::vector<int> &getCurrentEntityTexMask(const Entity &entity) const;

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
