#pragma once
#include "rend/texture.h"
#include "rend/shader.h"
#include "rend/mesh2d.h"

struct Parking2D {
	Texture* tex_pixel;
	Texture* tex_parking_2d;
	Mesh2D* mesh_2d;
	Mesh2D* mesh_circle;

	Parking2D(Mesh2D* mesh_2d, Mesh2D* mesh_circle, Texture* tex_pixel, Texture* tex_parking_2d);
	void draw(Shader* shd);
};