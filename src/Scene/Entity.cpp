#include "../Headers/Entity.h"

// =============
// Constructors
// =============

Entity::Entity(float x, float y, float angle, std::string *tex, int initHP,
               EntityState initState)
    : _id(-1), _transform{vec2f{x, y}, angle}, _texMapper(*tex),
      _health(initHP), _state(initState) {}

Entity::Entity(Transform pos, std::string *tex, int initHP,
               EntityState initState)
    : _id(-1), _transform(pos), _texMapper(*tex), _health(initHP),
      _state(initState) {}

// =============
// Process
// =============
void Entity::process() {}

// =============
// Getters
// =============
int Entity::ID() const { return _id; }
float Entity::X() const { return _transform.position.x; }
float Entity::Y() const { return _transform.position.y; }
float Entity::Angle() const { return _transform.angle; }

vec2f Entity::coordinates() { return _transform.position; }

const Transform &Entity::transform() const { return _transform; }
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

void Entity::setX(int x) { _transform.position.x = x; }
void Entity::setY(int y) { _transform.position.y = y; }

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
