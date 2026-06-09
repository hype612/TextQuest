#ifndef PLAYER_H
#define PLAYER_H

#include "./MapManager.h"
#include <cmath>
#include <string>
#include <tuple>

class Player {
public:
  Player(float x_pos, float y_pos, float angle, float fov,
         MapManager &mapManager);

  void set_x(float new_x);
  void addto_x(float rval_x);
  void subtractf_x(float rval_x);

  void set_y(float new_y);
  void addto_y(float rval_y);
  void subtractf_y(float rval_y);

  void set_pos(float new_x, float new_y);

  void set_angle(float new_a);
  void addto_angle(float rval_a);
  void subtractf_angle(float rval_a);

  float get_x() const;
  float get_y() const;
  float get_angle() const;
  float get_fov() const;
  float get_fov_rad() const;
  std::tuple<float, float> get_angle_unitv() const;

private:
  float m_x_pos;
  float m_y_pos;
  float m_angle;
  float m_fov;

  MapManager &_mapManager;
};

#endif
