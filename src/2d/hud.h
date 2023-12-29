#pragma once

#include "bbox.h"
#include <vector> 
#include "rend/mesh2d.h"
#include "rend/shader.h"

struct Hud {
	float scale = 0.75;
	struct {
		glm::vec2 pos;
		std::vector<BBox> cam_buttons;
		Texture* tex = nullptr;
		int button_flash_timer = 0;
	} map;

	Mesh2D* mesh_2d;

	Hud(Mesh2D* mesh_2d, Texture* map_tex);
	void init();
	void update();
	void draw(Shader* shd);
};