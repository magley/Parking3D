#pragma once

#include "bbox.h"
#include <vector> 
#include "rend/mesh2d.h"
#include "rend/shader.h"

struct CamButton {
	BBox bbox;
	int cam_index;

	CamButton(BBox bbox, int cam_index) : bbox(bbox), cam_index(cam_index) {}
};

struct Hud {
	float scale = 1;
	int help_visible = 2; // 0 = not visible, 1 = visible while button is held, 2 = visible until button is pressed
	struct {
		glm::vec2 pos = glm::vec2(0, 0);
		std::vector<CamButton> cam_buttons;
		Texture* tex = nullptr;
		int button_flash_timer = 0;
	} map;

	Mesh2D* mesh_2d;

	Hud(Mesh2D* mesh_2d, Texture* map_tex);
	void init();
	void update();
	void draw(Shader* shd);
};
