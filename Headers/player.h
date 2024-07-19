#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
	float m_x_pos;
	float m_y_pos;
	float m_angle;
	float m_fov;

	Player();
public:
	Player(float x_pos, float y_pos, float angle, float fov);

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

	/*
	float get_x();
	float get_y();
	float get_angle();
	float get_fov();
	*/

	float get_x() const { return m_x_pos; }
	float get_y() const { return m_y_pos; }
	float get_angle() const { return m_angle; }
	float get_fov() const { return m_fov; }
};




#endif