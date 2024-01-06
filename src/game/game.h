#pragma once

#include <vector>
#include "parking_spot.h"

struct Game {
	int _cam_index = 0;
	bool lock_cursor = false;

	struct {
		int seizure_min = 10;
		int seizure_max = 200;
		float intensity = 0.1;
		float seizure;
		int rand;
	} noise;

	ParkingSpot parking_spots[6];

	void set_cam(int index);
	void update_noise();
	void open_cam();
	void close_cam();
};