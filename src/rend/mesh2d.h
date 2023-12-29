#pragma once
#include "shader.h"
#include "texture.h"

struct Mesh2D {
	enum Type {
		SQUARE,
	};
	unsigned vbo;
	unsigned vao;
	int vertices_count;

	Mesh2D(Type type);
	void draw(const Shader* shd, Texture* texture);
private:
	void init_square();
};