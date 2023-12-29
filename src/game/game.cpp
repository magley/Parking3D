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
	}
}