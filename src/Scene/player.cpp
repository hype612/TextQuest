#include "../Headers/player.h"
#include <cmath>
#include <tuple>

Player::Player(float x_pos, float y_pos, float angle, float fov,
               MapManager &mapManager)
    : _posX(x_pos), _posY(y_pos), _angle(angle), _fov(fov),
      _directionalSpeeds{}, _mapManager(mapManager) {}

//====================
// Absolute setters
//====================

void Player::setX(float new_x) { _posX = new_x; }
void Player::setY(float new_y) { _posY = new_y; }
void Player::setAngle(float new_a) { _angle = new_a; }

void Player::setPos(float new_x, float new_y) {
  _posX = new_x;
  _posY = new_y;
}

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
  _posX += rval_x;
  if (_mapManager.isOutOfBounds((int)_posX, (int)_posY) ||
      _mapManager.isWall((int)_posX, (int)_posY))
    _posX -= rval_x;
}

void Player::addToY(float rval_y) {
  _posY += rval_y;
  if (_mapManager.isWall((int)_posX, (int)_posY) ||
      _mapManager.isOutOfBounds((int)_posX, (int)_posY))
    _posY -= rval_y;
}

void Player::addToAngle(float rval_a) { _angle += rval_a; }

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
/*
void Player::moveForward(float delta) {
  addToX(sinf(_angle) *
         _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] * delta);
  addToY(cosf(_angle) *
         _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] * delta);
}
void Player::moveBackward(float delta) {
  addToX(-1 * sinf(_angle) *
         _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] * delta);
  addToY(-1 * cosf(_angle) *
         _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] * delta);
}
void Player::moveRight(float delta) {
  addToX(sinf(_angle + (3.14159f / 2.0f)) *
         _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_RIGHT)] *
         delta);
  addToY(cosf(_angle + (3.14159f / 2.0f)) *
         _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_RIGHT)] *
         delta);
}
void Player::moveLeft(float delta) {
  addToX(sinf(_angle - (3.14159f / 2.0f)) *
         _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] *
         delta);
  addToY(cosf(_angle - (3.14159f / 2.0f)) *
         _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] *
         delta);
}
void Player::turnRight(float delta) {
  addToAngle(_directionalSpeeds[static_cast<int>(MoveDirection::TURN_RIGHT)] *
             delta);
}
void Player::turnLeft(float delta) {
  addToAngle(-1 *
             _directionalSpeeds[static_cast<int>(MoveDirection::TURN_LEFT)] *
             delta);
}
*/
void Player::move(float delta, MoveDirection dir) {
  switch (dir) {
  case MoveDirection::FORWARD:
    addToX(sinf(_angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] *
           delta);
    addToY(cosf(_angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::FORWARD)] *
           delta);
    break;
  case MoveDirection::BACKWARD:
    addToX(-1 * sinf(_angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] *
           delta);
    addToY(-1 * cosf(_angle) *
           _directionalSpeeds[static_cast<int>(MoveDirection::BACKWARD)] *
           delta);
    break;
  case MoveDirection::STRAFE_LEFT:
    addToX(sinf(_angle - (3.14159f / 2.0f)) *
           _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] *
           delta);
    addToY(cosf(_angle - (3.14159f / 2.0f)) *
           _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_LEFT)] *
           delta);
    break;
  case MoveDirection::STRAFE_RIGHT:
    addToX(sinf(_angle + (3.14159f / 2.0f)) *
           _directionalSpeeds[static_cast<int>(MoveDirection::STRAFE_RIGHT)] *
           delta);
    addToY(cosf(_angle + (3.14159f / 2.0f)) *
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

  //====================
  // Getters
  //====================

  float Player::getX() const { return _posX; }
  float Player::getY() const { return _posY; }
  float Player::getAngle() const { return _angle; }
  float Player::getFov() const { return _fov; }
  float Player::getFovInRad() const { return (_fov * 3.14159f) / 180; }
  float Player::getMoveSpeedOnDirection(MoveDirection direction) const {
    return _directionalSpeeds[static_cast<int>(direction)];
  }
  const std::array<float, moveDirectionCount> &Player::getMoveSpeedArray()
      const {
    return _directionalSpeeds;
  }
  std::tuple<float, float> Player::getAngleUnitVector() const {
    return std::tuple<float, float>(std::cos(_angle), -std::sin(_angle));
  }
