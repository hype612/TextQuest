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

void EntityManager::process()
{
  for(Entity e : _entityContainer) {
    e.process();
  }
}


// ways to access the container
void EntityManager::addEntity(Entity& entity)
{
  if (entity.ID() != -1)
  {
    std::cerr << "id has been altered. Please do not touch id" << std::endl;
    return;
  } 
  entity.setID(_entityContainer.size());
  _entityContainer.push_back(entity);
}
void EntityManager::removeEntity(int id) 
{
  _entityContainer[id].setX(-1);
  _entityContainer[id].setY(-1);
}


void EntityManager::removeEntity(int coord_x, int coord_y)
{
  int id = getEntityAtPos(coord_x, coord_y);
  removeEntity(id);
}


void EntityManager::removeAllEntities()
{
  _entityContainer.clear();
  _entityContainer.shrink_to_fit();
}



// for renderer
void EntityManager::changeEntityTextureScale(int EntityId, float distance)
{
  _entityContainer[EntityId].rescaleTexture(distance);
}


void EntityManager::changeEntityTextureScale(Entity& entity, float distance)
{
  entity.rescaleTexture(distance);
}


std::wstring EntityManager::getCurrentEntityTexture(int EntityId)
{
  _entityContainer[EntityId].getTexture();
}


std::wstring EntityManager::getCurrentEntityTexture(const Entity& entity)
{
  entity.getTexture(); 
}


std::wstring getCurrentEntityTexMask(int EntityId)
{
  _entityContainer[EntityId].getTexMask();
}


std::wstring getCurrentEntityTexMask(const Entity& entity)
{
  entity.getTexMask();
}


std::wstring getNextEntityCharColumn(int EntityId)
{
  _entityContainer[EntityId].getNextTexColumn();
}


std::wstring getEntityMaskColumn(int EntityId)
{
  _entityContainer[EntityId].getMaskColumn();
}
