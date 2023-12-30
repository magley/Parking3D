#pragma once
#include "shader.h"
#include "texture.h"

struct Mesh2D {
	enum Type {
		SQUARE,
		CIRCLE
	};
	unsigned vbo;
	unsigned vao;
	int vertices_count;
	Type type;

	Mesh2D(Type type);
	void draw(const Shader* shd, Texture* texture);
private:
	void init_square();
	void init_circle();
};