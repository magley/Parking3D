#include "game.h"

#include "subsystem/subsystem_audio.h"
#include "subsystem/subsystem_entity.h"
#include "subsystem/subsystem_resource.h"

void Game::setup_cam_indices(int free_cam_index, int down_cam_index) {
	this->free_cam_index = free_cam_index;
	this->down_cam_index = down_cam_index;
}

std::vector<Entity*> Game::get_camera_entities() {
	std::vector<Entity*> entities;
	for (Entity* e : glo::entity->arr) {
		if (e->has(Component::CAM)) {
			entities.push_back(e);
		}
	}

	return entities;
}

void Game::set_cam(int index) {
	int prev_cam_index = curr_cam_index;
	const auto cam_entities = get_camera_entities();

	for (int i = 0; i < cam_entities.size(); i++) {
		Entity* e = cam_entities[i];
		e->cam.active = (i == index);
	}

	curr_cam_index = index;

	// Toggle cursor lock.

	Entity* e = cam_entities[curr_cam_index];
	if (e->cam.active == true) {
		lock_cursor = e->cam.type == CCam::FREE || e->cam.type == CCam::FREE_STATIONARY;
	}

	// Effects

	if (is_cctv_cam(curr_cam_index)) {
		noise.seizure_min = 0;
		noise.intensity = 2.0;

		WavSample* snd_cam_switch = glo::res->load_wav("cam_switch.wav");
		glo::audio->play(snd_cam_switch);
		if (!is_cctv_cam(prev_cam_index)) {
			open_cam();
		}
	}
	else {
		noise.intensity = 0;
		if (is_cctv_cam(prev_cam_index)) {
			close_cam();
		}
	}
}

void Game::next_cam(bool previous) {
	const auto cam_entities = get_camera_entities();
	const int k = previous ? -1 : 1;
	int new_index = curr_cam_index;

	while (true) {
		new_index = (new_index + k + cam_entities.size()) % cam_entities.size();

		if (!allow_fun_cams) {
			if (new_index == free_cam_index || new_index == down_cam_index) {
				continue;
			}
		}
		break;
	}

	set_cam(new_index);
}

bool Game::is_cctv_cam(int index) {
	return index != free_cam_index && index != down_cam_index;
}

/*
void Game::set_cam(int index) {
	int prev_cam_index = _cam_index;
	_cam_index = index;

	std::vector<int> cam_entity_index;
	for (int i = 0; i < glo::entity->size(); i++) {
		Entity* e = glo::entity->arr[i];
		if (e->has(Component::CAM)) {
			cam_entity_index.push_back(i);
		}
	}

	_cam_index %= cam_entity_index.size();
	if (_cam_index < 0) {
		_cam_index = cam_entity_index[cam_entity_index.size() - 1];
	}
	if (_cam_index > cam_entity_index.size() - 1) {
		_cam_index = cam_entity_index[0];
	}
	if (!allow_fun_views) {
		if (_cam_index == 0) {
			_cam_index = 1;
		}
		if (_cam_index == 5) {
			_cam_index = 4;
		}
	}
	int cam_entity_target_index = cam_entity_index[_cam_index];

	for (int i = 0; i < cam_entity_index.size(); i++) {
		int index = cam_entity_index[i];
		Entity* e = glo::entity->arr[index];
		e->cam.active = (index == cam_entity_target_index);

		if (e->cam.active == true) {
			lock_cursor = e->cam.type == CCam::FREE || e->cam.type == CCam::FREE_STATIONARY;
		}
	}

	if (_cam_index != 0 && _cam_index != 5) {
		noise.seizure_min = 0;
		noise.intensity = 2.0;

		WavSample* snd_cam_switch = glo::res->load_wav("cam_switch.wav");
		glo::audio->play(snd_cam_switch);
		if (prev_cam_index == 0 || prev_cam_index == 5) {
			open_cam();
		}
	}
	else {
		noise.intensity = 0;
		if (prev_cam_index != 0 && prev_cam_index != 5) {
			close_cam();
		}
	}
}
*/

void Game::update_noise() {
	if (noise.seizure_min < 50) {
		noise.seizure_min += 1;
	}

	if (noise.intensity > 0.2) {
		noise.intensity -= 0.04f;
	}

	noise.rand = rand() / 9;
	noise.seizure = (rand() % (noise.seizure_min - noise.seizure_max)) + (float)noise.seizure_min;
}

void Game::open_cam() {
	Entity* mini_screen = nullptr;
	for (Entity* e : glo::entity->arr) {
		if (e->has(Component::MINISCREEN)) {
			mini_screen = e;
			break;
		}
	}

	if (mini_screen == nullptr) {
		return;
	}

	mini_screen->miniscreen.raise(mini_screen);
}

void Game::close_cam() {
	Entity* mini_screen = nullptr;
	for (Entity* e : glo::entity->arr) {
		if (e->has(Component::MINISCREEN)) {
			mini_screen = e;
			break;
		}
	}

	if (mini_screen == nullptr) {
		return;
	}

	mini_screen->miniscreen.lower(mini_screen);
}