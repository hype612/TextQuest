#ifndef PLAYER_H
#define PLAYER_H

#include "./MapManager.h"
#include "./MoveDirection.h"
#include "Transform.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <tuple>

class Player {
public:
  Player(float x_pos, float y_pos, float angle, float fov,
         MapManager &mapManager);

  //====================
  // Absolute setters
  //====================

  void setX(float new_x);
  void setY(float new_y);
  void setAngle(float new_a);
  void setPos(const vec2f &new_pos);
  void setMoveSpeedOnDirection(float new_speed, MoveDirection direction);
  void setMoveSpeedAllDirectons(float new_speed);
  void setTurnSpeedAlldirections(float new_speed);
  //====================
  // Relative setters
  //====================

  void addToX(float rval_x);
  void addToY(float rval_y);
  void addToAngle(float rval_a);
  void addToMoveSpeedOnDirection(float new_speed, MoveDirection direction);
  void addToMoveSpeedAllDirections(float new_speed);
  void addToTurnSpeedAlldirections(float new_speed);

  void move(float delta, MoveDirection dir);

  //====================
  // Getters
  //====================

  float getX() const;
  float getY() const;
  float getAngle() const;
  float getFov() const;
  float getFovInRad() const;
  float getMoveSpeedOnDirection(MoveDirection direction) const;
  const std::array<float, moveDirectionCount> &getMoveSpeedArray() const;
  std::tuple<float, float> getAngleUnitVector() const;

private:
  float _posX;
  float _posY;
  float _angle;
  Transform _transform;
  float _fov;

  std::array<float, moveDirectionCount> _directionalSpeeds;
  MapManager &_mapManager;
};

#endif
