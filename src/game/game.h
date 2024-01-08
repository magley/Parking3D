#pragma once

#include <vector>
#include "parking_spot.h"

struct Entity;

struct Game {
	int curr_cam_index;
	int free_cam_index; // Readonly
	int down_cam_index; // Readonly
	bool allow_fun_cams = false;
	void setup_cam_indices(int free_cam_index, int down_cam_index);
	void next_cam(bool previous);
	bool is_cctv_cam(int index);

	bool lock_cursor = false;

	struct {
		int seizure_min = 10;
		int seizure_max = 200;
		float intensity = 0.1f;
		float seizure = 0;
		int rand = 0;
	} noise;

	ParkingSpot parking_spots[6] = {};

	void set_cam(int index);
	void update_noise();
	void open_cam();
	void close_cam();

	void update_mouse_freedom();
	void update_game_inputs();
	void update_car_spawn();
private:
	std::vector<Entity*> get_camera_entities();
};