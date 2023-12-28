#pragma once
#include "color.h"

struct Material {
	Color ambient;
	Color diffuse;
	Color specular;
	float shininess;
};