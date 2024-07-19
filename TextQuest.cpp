#include <iostream>
#include <Windows.h>
#include <chrono>
#include "Headers/player.h"
#include <vector>
#include <algorithm>

// screen consts
int screen_width = 240; // 120
int screen_height = 80; // 40

// map consts
int map_height = 16;
int map_width = 16;
float max_raylength = 16.0f;

void run_game() {
	Player player(8.0f, 8.0f, 6.28f, 3.14159/4.0f);
	wchar_t *screen = new wchar_t[screen_width * screen_height];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// creating topology
	std::wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"##########.....#";
	map += L"#..............#";
	map += L"#....###########";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"######....######";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";

	// game loop

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();
	
	float player_fov = player.get_fov();
	
	while (true) {
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed_time = tp2 - tp1;
		tp1 = tp2;
		float f_elapsed_time = elapsed_time.count();
		
		// movements
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
			// player_a -= 0.8f * f_elapsed_time;
			player.subtractf_angle(0.8f * f_elapsed_time);
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
			// player_a += 0.8f * f_elapsed_time;
			player.addto_angle(0.8f * f_elapsed_time);
		}
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
			player.addto_x(sinf(player.get_angle()) * 5.0f * f_elapsed_time);
			player.addto_y(cosf(player.get_angle()) * 5.0f * f_elapsed_time);

			if (map[(int)player.get_y() * map_width + (int)player.get_x()] == '#') {
				player.subtractf_x(sinf(player.get_angle()) * 5.0f * f_elapsed_time);
				player.subtractf_y(cosf(player.get_angle()) * 5.0f * f_elapsed_time);
			}
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
			player.subtractf_x(sinf(player.get_angle()) * 5.0f * f_elapsed_time);
			player.subtractf_y(cosf(player.get_angle()) * 5.0f * f_elapsed_time);
			
			if (map[(int)player.get_y() * map_width + (int)player.get_x()] == '#') {
				player.addto_x(sinf(player.get_angle()) * 5.0f * f_elapsed_time);
				player.addto_y(cosf(player.get_angle()) * 5.0f * f_elapsed_time);
			}
		}
		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) {
			player.addto_x(sinf(player.get_angle() - (3.14159 / 2)) * 5.0f * f_elapsed_time);
			player.addto_y(cosf(player.get_angle() - (3.14159 / 2)) * 5.0f * f_elapsed_time);

			if (map[(int)player.get_y() * map_width + (int)player.get_x()] == '#') {
				player.subtractf_x(sinf(player.get_angle() - (3.14159 / 2)) * 5.0f * f_elapsed_time);
				player.subtractf_y(cosf(player.get_angle() - (3.14159 / 2)) * 5.0f * f_elapsed_time);
			}
		}
		if (GetAsyncKeyState((unsigned short)'E') & 0x8000) {
			player.addto_x(sinf(player.get_angle() + (3.14159 / 2)) * 5.0f * f_elapsed_time);
			player.addto_y(cosf(player.get_angle() + (3.14159 / 2)) * 5.0f * f_elapsed_time);

			if (map[(int)player.get_y() * map_width + (int)player.get_x()] == '#') {
				player.subtractf_x(sinf(player.get_angle() + (3.14159 / 2)) * 5.0f * f_elapsed_time);
				player.subtractf_y(cosf(player.get_angle() + (3.14159 / 2)) * 5.0f * f_elapsed_time);
			}
		}
		
		bool hitwall;
		bool isedge;
		
		// actual raytracing
		for (int x = 0; x < screen_width; x++) {
			float ray_angle = (player.get_angle() - player_fov / 2.0f) + ((float)x / (float)screen_width) * player_fov;
			float distance_to_wall = 0.0f;
			hitwall = false;
			isedge = false;
			
			float eye_x = sinf(ray_angle);
			float eye_y = cosf(ray_angle);

			while (!hitwall && distance_to_wall < max_raylength) {
				distance_to_wall += 0.1f;
				int test_x = (int)(player.get_x() + eye_x * distance_to_wall);
				int test_y = (int)(player.get_y() + eye_y * distance_to_wall);
				if (test_x < 0 || test_x >= map_width || test_y < 0 || test_y >= map_height) {
					hitwall = true;
					distance_to_wall = max_raylength; // imo could cause weird rendering
													  // long distance. check later
				}
				else {
					// ray is inbounds > test if is a wall block
					if (map[test_y * map_width + test_x] == '#') {
						hitwall = true;
				
						std::vector<std::pair<float, float>> edges;
						for( int tx = 0; tx < 2; tx++) {
							for( int ty = 0; ty < 2; ty++) { 
								float vy = (float)test_y + ty - player.get_y();
								float vx = (float)test_x + tx - player.get_x();
								float d = sqrt(vx * vx + vy * vy);
								float dot = (eye_x * vx / d) + (eye_y * vy / d);
								edges.push_back(std::make_pair(d, dot));
								sort(edges.begin(), edges.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) { return left.first < right.first; });
							}
						}
						float edge_bound = 0.002f;
						if (acos(edges.at(0).second) < edge_bound) isedge = true;
						if (acos(edges.at(1).second) < edge_bound) isedge = true;
					}
				}
			}
			// ray hit a wall
			int ceiling = (float)(screen_height / 2.0f) - screen_height / ((float)distance_to_wall);
			int floor = screen_height - ceiling;

			// wall shade
			short shade;


				
			for (int y = 0; y < screen_height; y++) {
				if (y < ceiling)
					screen[y * screen_width + x] = ' ';
				else if (y > ceiling && y <= floor)
				{
					if (distance_to_wall <= max_raylength / 4.0f)		shade = 0x2588;
					else if (distance_to_wall < max_raylength / 3.0f)	shade = 0x0040;
					else if (distance_to_wall < max_raylength / 2.0f)	shade = 0x007C;
					else												shade = ' ';
					if (isedge && distance_to_wall < max_raylength / 2.0f) shade = 'e';
					screen[y * screen_width + x] = shade;
				}
				else {
					float b = 1.0f - (((float)y - screen_height / 2.0f) / ((float)screen_height / 2.0f));
					if (b < 0.25)		shade = '#';
					else if (b < 0.5)	shade = 'X';
					else if (b < 0.75)	shade = '.';
					else if (b < 0.9)	shade = '-';
					else				shade = ' ';
					screen[y * screen_width + x] = shade;
				}
			}
		}
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f, FPS=%3.2f", player.get_x(), player.get_y(), player.get_angle(), 1.0f / f_elapsed_time);

		screen[screen_height * screen_width - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, screen_width * screen_height, { 0,0 }, &dwBytesWritten);
	}


}


int main() {
	run_game();
	
	// write 2 screen
	return 0;
}