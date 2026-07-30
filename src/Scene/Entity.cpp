#include "../Headers/Entity.h"
#include "../Headers/IBehaviorController.h"
#include "../Headers/Vec2f.h"
#include <cmath>

// =============
// Constructors
// =============

Entity::Entity(std::unique_ptr<IBehaviorController> behaviourCtrl,
               Transform pos, std::string *tex, int initHP)
    : _id(-1), _transform(pos), _texMapper(*tex), _health(initHP),
      _behaviourCtrl(std::move(behaviourCtrl)) {}

// =============
// Process
// =============
vec2f Entity::process(float delta) {
  return _behaviourCtrl->Tick(*this, delta);
}

// =============
// Getters
// =============
int Entity::ID() const { return _id; }

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

std::string Entity::scaledTex(unsigned int width, unsigned int height,
                              int shadingIdx) const {
  return _texMapper.scaledTex(width, height, shadingIdx);
}
std::vector<int> Entity::scaledMask(unsigned int width,
                                    unsigned int height) const {
  return _texMapper.scaledMask(width, height);
}
int Entity::getHP() { return _health; }
float Entity::moveSpeedOnDir(MoveDirection direction) const {
  return _directionalSpeeds[static_cast<int>(direction)];
}
// =============
// Setters
// =============

void Entity::addToX(float rval_x) { _transform.position.x += rval_x; }

void Entity::addToY(float rval_y) { _transform.position.y += rval_y; }

void Entity::addToAngle(float rval_a) { _transform.angle += rval_a; }

void Entity::addToMoveSpeedOnDirection(float new_speed,
                                       MoveDirection direction) {
  _directionalSpeeds[static_cast<int>(direction)] += new_speed;
}

void Entity::addToMoveSpeedAllDirections(float new_speed) {
  _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] += new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] += new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] += new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_RIGHT)] +=
      new_speed;
}
void Entity::addToTurnSpeedAlldirections(float new_speed) {
  _directionalSpeeds[static_cast<int>(MoveDirection::TURN_LEFT)] += new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::TURN_RIGHT)] += new_speed;
}

void Entity::setMoveSpeedOnDirection(float new_speed, MoveDirection direction) {
  _directionalSpeeds[static_cast<int>(direction)] = new_speed;
}

void Entity::setMoveSpeedAllDirectons(float new_speed) {
  _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] = new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] = new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] = new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_RIGHT)] = new_speed;
}
void Entity::setTurnSpeedAlldirections(float new_speed) {
  _directionalSpeeds[static_cast<int>(MoveDirection::TURN_LEFT)] = new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::TURN_RIGHT)] = new_speed;
}

void Entity::move(float delta, MoveDirection dir) {
  switch (dir) {
  case MoveDirection::FORWARD:
    addToX(sin(_transform.angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] *
           delta);
    addToY(cos(_transform.angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] *
           delta);
    break;
  case MoveDirection::BACKWARD:
    addToX(-1 * sin(_transform.angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] *
           delta);
    addToY(-1 * cos(_transform.angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] *
           delta);
    break;
  case MoveDirection::STRAFE_LEFT:
    addToX(sin(_transform.angle - (3.14159f / 2.0f)) *
           _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] *
           delta);
    addToY(cos(_transform.angle - (3.14159f / 2.0f)) *
           _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] *
           delta);
    break;
  case MoveDirection::STRAFE_RIGHT:
    addToX(sin(_transform.angle + (3.14159f / 2.0f)) *
           _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_RIGHT)] *
           delta);
    addToY(cos(_transform.angle + (3.14159f / 2.0f)) *
           _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_RIGHT)] *
           delta);
    break;
  case MoveDirection::TURN_LEFT:
    addToAngle(-1 *
               _directionalSpeeds[static_cast<int>(MoveDirection::TURN_LEFT)] *
               delta);
    break;
  case MoveDirection::TURN_RIGHT:
    addToAngle(_directionalSpeeds[static_cast<int>(MoveDirection::TURN_RIGHT)] *
               delta);
    break;
  }
}

void Entity::setID(int id) {
  if (_id == -1)
    _id = id;
  return;
}

void Entity::setTransform(const Transform &new_transform) {
  _transform = new_transform;
}

void Entity::modifyHealth(int amount) {
  _health += amount;
  if (_health <= 0) {
    // destroy object
  }
}
