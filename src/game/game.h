#pragma once

struct Game {
	int _cam_index = 0;
	bool lock_cursor = false;

	struct {
		int seizure_min = 20;
		int seizure_max = 200;
		float intensity = 0.1;
		float seizure;
		int rand;
	} noise;

	void set_cam(int index);
	void update_noise();
};