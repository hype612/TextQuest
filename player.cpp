#include "Headers/player.h"

Player::Player(float x_pos, float y_pos, float angle, float fov) :
	m_x_pos(x_pos), m_y_pos(y_pos), m_angle(angle), m_fov(fov) {}

void Player::set_x(float new_x) {
	m_x_pos = new_x;
}


void Player::addto_x(float rval_x) {
	m_x_pos += rval_x;
}
void Player::subtractf_x(float rval_x) {
	m_x_pos -= rval_x;
}

void Player::set_y(float new_y) {
	m_y_pos = new_y;
}

void Player::addto_y(float rval_y) {
	m_y_pos += rval_y;
}
void Player::subtractf_y(float rval_y) {
	m_y_pos -= rval_y;
}

void Player::set_pos(float new_x, float new_y) {
	m_x_pos = new_x;
	m_y_pos = new_y;
}

void Player::set_angle(float new_a) {
	m_angle = new_a;
}

void Player::addto_angle(float rval_a) {
	m_angle += rval_a;
}

void Player::subtractf_angle(float rval_a) {
	m_angle -= rval_a;
}

/*
float Player::get_x() { return m_x_pos; }
float Player::get_y() { return m_y_pos; }
float Player::get_angle() { return m_angle; }
float Player::get_fov() { return m_fov; }
*/
