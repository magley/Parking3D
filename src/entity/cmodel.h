#pragma once
#include "rend/model.h"
#include "rend/shader.h"

struct Entity;

struct CModel {
	Model* mdl = nullptr;
	Shader* shd = nullptr;
	bool transparent = false;
	bool visible = true;

	void draw(Entity* self);
};