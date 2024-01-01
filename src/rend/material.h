#pragma once
#include "color.h"

struct Material {
	Color ambient;
	Color diffuse;
	Color specular;
	float shininess;

	Texture* diffuse_map;
	Texture* specular_map;
	Texture* emission_map;
	Texture* normal_map;
};