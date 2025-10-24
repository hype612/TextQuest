#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
private:
	float m_x_pos;
	float m_y_pos;
	float m_angle;
	float m_fov;
	
	std::wstring& m_map;
	int _map_width;
	int _map_height;

public:
	Player(float x_pos, float y_pos, float angle, float fov, std::wstring& map, int map_width, int map_height);
	Player();

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


	float get_x() const { return m_x_pos; }
	float get_y() const { return m_y_pos; }
	float get_angle() const { return m_angle; }
	float get_fov() const { return m_fov; }
};




#endif
