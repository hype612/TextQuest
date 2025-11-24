#include "../Headers/Entity.h"

// =============
// Constructors
// =============

Entity::Entity(int x, int y, std::wstring *tex, int initHP,
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

std::wstring Entity::getTexture() const { return _texMapper.getTexture(); }

std::wstring Entity::getNextTexColumn(int height) {
  return _texMapper.getNextTexColumn(height);
}

const std::vector<int> &Entity::getTexMask() const {
  return _texMapper.getMask();
}

std::vector<int> Entity::getMaskColumn(int height) const {
  return _texMapper.getMaskColumn(height);
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

void Entity::rescaleTexture(float distance) {
  _texMapper.rescaleCurrentTexture(distance);
}
