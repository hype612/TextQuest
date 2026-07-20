#include "../Headers/player.h"
#include <cmath>
#include <tuple>

Player::Player(float x_pos, float y_pos, float angle, MapManager &mapManager)
    : _transform{{x_pos, y_pos}, angle}, _directionalSpeeds{},
      _mapManager(mapManager) {}

Player::Player(const Transform &pos, MapManager &mapManager)
    : _transform(pos), _directionalSpeeds{}, _mapManager(mapManager) {}

//====================
// Absolute setters
//====================

void Player::setX(float new_x) { _transform.position.x = new_x; }
void Player::setY(float new_y) { _transform.position.y = new_y; }
void Player::setAngle(float new_a) { _transform.angle = new_a; }

void Player::setPos(const vec2f &new_pos) { _transform.position = new_pos; }

void Player::setMoveSpeedOnDirection(float new_speed, MoveDirection direction) {
  _directionalSpeeds[static_cast<int>(direction)] = new_speed;
}

void Player::setMoveSpeedAllDirectons(float new_speed) {
  _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] = new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] = new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] = new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_RIGHT)] = new_speed;
}
void Player::setTurnSpeedAlldirections(float new_speed) {
  _directionalSpeeds[static_cast<int>(MoveDirection::TURN_LEFT)] = new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::TURN_RIGHT)] = new_speed;
}

//====================
// Relative setters
//====================

void Player::addToX(float rval_x) {
  _transform.position.x += rval_x;
  if (_mapManager.isOutOfBounds((int)_transform.position.x,
                                (int)_transform.position.y) ||
      _mapManager.isWall((int)_transform.position.x,
                         (int)_transform.position.y))
    _transform.position.x -= rval_x;
}

void Player::addToY(float rval_y) {
  _transform.position.y += rval_y;
  if (_mapManager.isWall((int)_transform.position.x,
                         (int)_transform.position.y) ||
      _mapManager.isOutOfBounds((int)_transform.position.x,
                                (int)_transform.position.y))
    _transform.position.y -= rval_y;
}

void Player::addToAngle(float rval_a) { _transform.angle += rval_a; }

void Player::addToMoveSpeedOnDirection(float new_speed,
                                       MoveDirection direction) {
  _directionalSpeeds[static_cast<int>(direction)] += new_speed;
}

void Player::addToMoveSpeedAllDirections(float new_speed) {
  _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] += new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] += new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] += new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_RIGHT)] +=
      new_speed;
}
void Player::addToTurnSpeedAlldirections(float new_speed) {
  _directionalSpeeds[static_cast<int>(MoveDirection::TURN_LEFT)] += new_speed;
  _directionalSpeeds[static_cast<int>(MoveDirection::TURN_RIGHT)] += new_speed;
}
void Player::move(float delta, MoveDirection dir) {
  switch (dir) {
  case MoveDirection::FORWARD:
    addToX(sinf(_transform.angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] *
           delta);
    addToY(cosf(_transform.angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] *
           delta);
    break;
  case MoveDirection::BACKWARD:
    addToX(-1 * sinf(_transform.angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] *
           delta);
    addToY(-1 * cosf(_transform.angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] *
           delta);
    break;
  case MoveDirection::STRAFE_LEFT:
    addToX(sinf(_transform.angle - (3.14159f / 2.0f)) *
           _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] *
           delta);
    addToY(cosf(_transform.angle - (3.14159f / 2.0f)) *
           _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] *
           delta);
    break;
  case MoveDirection::STRAFE_RIGHT:
    addToX(sinf(_transform.angle + (3.14159f / 2.0f)) *
           _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_RIGHT)] *
           delta);
    addToY(cosf(_transform.angle + (3.14159f / 2.0f)) *
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

//====================
// Getters
//====================

float Player::getX() const { return _transform.position.x; }
float Player::getY() const { return _transform.position.y; }
float Player::getAngle() const { return _transform.angle; }
float Player::getMoveSpeedOnDirection(MoveDirection direction) const {
  return _directionalSpeeds[static_cast<int>(direction)];
}
const std::array<float, moveDirectionCount> &Player::getMoveSpeedArray() const {
  return _directionalSpeeds;
}
std::tuple<float, float> Player::getAngleUnitVector() const {
  return std::tuple<float, float>(std::cos(_transform.angle),
                                  -std::sin(_transform.angle));
}
const Transform &Player::transform() const { return _transform; }
