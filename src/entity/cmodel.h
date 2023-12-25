#pragma once
#include "rend/model.h"
#include "rend/shader.h"

struct Entity;

struct CModel {
	Model* mdl = nullptr;
	Shader* shd = nullptr;

	void draw(Entity* self);
};