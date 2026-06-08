#include "../Headers/EntityManager.h"

EntityManager::EntityManager() {}

// naive searches, later add something to speed it up if neccessary
std::optional<std::reference_wrapper<Entity>>
EntityManager::getEntityAtPos(int coord_x, int coord_y) {
  for (Entity &e : _entityContainer) {
    if (e.X() == coord_x && e.Y() == coord_y)
      return e;
  }
  return std::nullopt;
}
int EntityManager::getEntityIdAtPos(int coord_x, int coord_y) const {
  for (const Entity &e : _entityContainer) {
    if (e.X() == coord_x && e.Y() == coord_y)
      return e.ID();
  }

  return -1;
}

int EntityManager::getEntityCount() const { return _entityContainer.size(); }

void EntityManager::process() {
  for (Entity &e : _entityContainer) {
    e.process();
  }
}

// ways to access the container
void EntityManager::addEntity(Entity &entity) {
  if (entity.ID() != -1) {
    std::cerr << "id has been altered. Please do not touch id" << std::endl;
    return;
  }
  entity.setID(_entityContainer.size());
  _entityContainer.push_back(entity);
}
void EntityManager::removeEntity(int id) {
  _entityContainer[id].setX(-1);
  _entityContainer[id].setY(-1);
}

void EntityManager::removeEntity(int coord_x, int coord_y) {
  int id = getEntityIdAtPos(coord_x, coord_y);
  removeEntity(id);
}

void EntityManager::removeAllEntities() {
  _entityContainer.clear();
  _entityContainer.shrink_to_fit();
}

std::string EntityManager::getCurrentEntityTexture(int EntityId) {
  return _entityContainer[EntityId].getTexture();
}

std::string EntityManager::getCurrentEntityTexture(const Entity &entity) {
  return entity.getTexture();
}

const std::vector<int> &EntityManager::getCurrentEntityTexMask(int EntityId) {
  return _entityContainer[EntityId].getTexMask();
}

const std::vector<int> &
EntityManager::getCurrentEntityTexMask(const Entity &entity) {
  return entity.getTexMask();
}

std::string EntityManager::getEntityTexColAt(int mapX, int mapY, int height,
                                             float hitpoint) {
  int id = getEntityIdAtPos(mapX, mapY);
  return _entityContainer[id].getTextColumnAt(height, hitpoint);
}

std::vector<int> EntityManager::getEntityMaskColAt(int mapX, int mapY,
                                                   int height, float hitpoint) {
  int id = getEntityIdAtPos(mapX, mapY);
  return _entityContainer[id].getMaskColumnAt(height, hitpoint);
}
