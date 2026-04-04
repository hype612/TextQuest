#include "../Headers/player.h"
#include <cmath>
#include <tuple>

Player::Player(float x_pos, float y_pos, float angle, float fov,
               MapManager &mapManager)
    : m_x_pos(x_pos), m_y_pos(y_pos), m_angle(angle), m_fov(fov),
      _mapManager(mapManager) {}

void Player::set_x(float new_x) { m_x_pos = new_x; }

void Player::set_y(float new_y) { m_y_pos = new_y; }

void Player::addto_x(float rval_x) {
  m_x_pos += rval_x;
  if (_mapManager.isWall((int)m_y_pos, (int)m_x_pos) ||
      _mapManager.isOutOfBounds((int)m_x_pos, (int)m_y_pos))
    m_x_pos -= rval_x;
}
void Player::subtractf_x(float rval_x) {
  m_x_pos -= rval_x;
  if (_mapManager.isWall((int)m_y_pos, (int)m_x_pos) ||
      _mapManager.isOutOfBounds((int)m_x_pos, (int)m_y_pos))
    m_x_pos += rval_x;
}

void Player::addto_y(float rval_y) {
  m_y_pos += rval_y;
  if (_mapManager.isWall((int)m_y_pos, (int)m_x_pos) ||
      _mapManager.isOutOfBounds((int)m_x_pos, (int)m_y_pos))
    m_y_pos -= rval_y;
}
void Player::subtractf_y(float rval_y) {
  m_y_pos -= rval_y;
  if (_mapManager.isWall((int)m_y_pos, (int)m_x_pos) ||
      _mapManager.isOutOfBounds((int)m_x_pos, (int)m_y_pos))
    m_y_pos += rval_y;
}

void Player::set_pos(float new_x, float new_y) {
  m_x_pos = new_x;
  m_y_pos = new_y;
}

void Player::set_angle(float new_a) { m_angle = new_a; }

void Player::addto_angle(float rval_a) { m_angle += rval_a; }

void Player::subtractf_angle(float rval_a) { m_angle -= rval_a; }

float Player::get_x() const { return m_x_pos; }
float Player::get_y() const { return m_y_pos; }
float Player::get_angle() const { return m_angle; }
float Player::get_fov() const { return m_fov; }

std::tuple<float, float> Player::get_angle_unitv() const {
  return std::tuple<float, float>(std::cos(m_angle), -std::sin(m_angle));
}
