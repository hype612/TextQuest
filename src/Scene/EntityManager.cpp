#include "../Headers/EntityManager.h"


EntityManager::EntityManager() {}


// naive searches, later add something to speed it up if neccessary
Entity& EntityManager::getEntityAtPos(int coord_x, int coord_y) {
  for(Entity e : _entityContainer) {
    if ( e.coord_x == coord_x && e.coord_y == coord_y)
      return e;
  }
}
int EntityManager::getEntityIdAtPos(int coord_x, int coord_y) {
  for(Entity e : _entityContainer) {
    if ( e.X() == coord_x && e.Y() == coord_y)
      return e.ID();
  }
}


void EntityManager::ScaleEntityTexture(int EntityId) {
  
}
void EntityManager::GetEntityTexture(int EntityId) {}

// ways to access the container
void EntityManager::addEntity(Entity& entity);
void EntityManager::removeEntity(int id);
void EntityManager::removeEntity(int coord_x, int coord_y);

// for renderer
void EntityManager::ChangeEntityTextureScale(int EntityId, float distance);
void EntityManager::ChangeEntityTextureScale(Entity& entity, float distance);
std::wstring EntityManager::GetCurrentEntityTexture(int EntityId);
std::wstring EntityManager::GetCurrentEntityTexture(const Entity& entity);


