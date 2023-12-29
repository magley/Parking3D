#include "game.h"
#include "global.h"

namespace glo {
	Game game = Game();
};

void Game::set_cam(int index) {
	_cam_index = index;

	std::vector<int> cam_entity_index;
	for (int i = 0; i < glo::wctx.entity.size(); i++) {
		Entity* e = glo::wctx.entity.arr[i];
		if (e->has(Component::CAM)) {
			cam_entity_index.push_back(i);
		}
	}

	_cam_index %= cam_entity_index.size();
	int cam_entity_target_index = cam_entity_index[_cam_index];

	for (int i = 0; i < cam_entity_index.size(); i++) {
		int index = cam_entity_index[i];
		Entity* e = glo::wctx.entity.arr[index];
		e->cam.active = (index == cam_entity_target_index);

		if (e->cam.active == true) {
			lock_cursor = e->cam.type == CCam::FREE;
		}
	}

	if (_cam_index != 0) {
		noise.seizure_min = 0;
		noise.intensity = 1.0;
	}
	else {
		noise.intensity = 0;
	}
}

void Game::update_noise() {
	if (noise.seizure_min < 50) {
		noise.seizure_min += 1;
	}

	if (noise.intensity > 0.2) {
		noise.intensity -= 0.01;
	}

	noise.rand = rand() / 9;
	noise.seizure = (rand() % (noise.seizure_min - noise.seizure_max)) + noise.seizure_min;
}