#include "../Headers/Entity.h"

// =============
// Constructors
// =============

Entity::Entity(int x, int y, std::string *tex, int initHP,
               EntityState initState)
    : _id(-1), _x(x), _y(y), _texMapper(*tex), _health(initHP),
      _state(initState) {}

// =============
// Process
// =============
void Entity::process() {}

// =============
// Getters
// =============
int Entity::ID() const { return _id; }
float Entity::X() const { return _x; }
float Entity::Y() const { return _y; }

int Entity::discreteX() const { return (int)_x; }
int Entity::discreteY() const { return (int)_y; }

std::tuple<int, int> Entity::getCoordinates() {
  return std::tuple<int, int>(_x, _y);
}

std::string Entity::getTexture() const { return _texMapper.getTexture(); }

const std::vector<int> &Entity::getTexMask() const {
  return _texMapper.getMask();
}

std::string Entity::getTexColumnAt(int height, float hitpoint,
                                   int wallTop) const {
  return getTexColumnAt(height, hitpoint, wallTop, 0);
}

std::string Entity::getTexColumnAt(int height, float hitpoint, int wallTop,
                                   int shadingIdx) const {
  return _texMapper.getTexColumnAt(height, hitpoint, wallTop, shadingIdx);
}

std::vector<int> Entity::getMaskColumnAt(int height, float hitpoint) {
  return _texMapper.getMaskColumnAt(height, hitpoint);
}

int Entity::getHP() { return _health; }

// =============
// Setters
// =============

void Entity::setX(int x) { _x = x; }
void Entity::setY(int y) { _y = y; }

void Entity::setID(int id) {
  if (_id == -1)
    _id = id;
  return;
}

void Entity::modifyHealth(int amount) {
  _health += amount;
  if (_health <= 0) {
    // destroy object
  }
}
