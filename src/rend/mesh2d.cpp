#include "mesh2d.h"
#include "GL/glew.h"
#include <glm/glm.hpp>

Mesh2D::Mesh2D(Mesh2D::Type type): type(type) {
	switch (type) {
	case SQUARE:
		init_square();
		return;
	case CIRCLE:
		init_circle();
		return;
	}
}

void Mesh2D::draw(const Shader* shd, Texture* texture) {
	glUseProgram(shd->prog);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture->tex);

	switch (type) {
	case SQUARE:
		glDrawArrays(GL_TRIANGLES, 0, vertices_count);
		return;
	case CIRCLE:
		glDrawArrays(GL_POLYGON, 0, vertices_count);
		return;
	}
}

void Mesh2D::init_square() {
	float vertices[] = {
		-0.5 + 0.5, -0.5 - 0.5,		0.0, 1.0,
		+0.5 + 0.5, -0.5 - 0.5,		1.0, 1.0,
		+0.5 + 0.5, +0.5 - 0.5,		1.0, 0.0,

		-0.5 + 0.5, -0.5 - 0.5,		0.0, 1.0,
		+0.5 + 0.5, +0.5 - 0.5,		1.0, 0.0,
		-0.5 + 0.5, +0.5 - 0.5,		0.0, 0.0,
	};
	vertices_count = 6;

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh2D::init_circle() {
	float vertices[100 * 4] = { 0 };
	vertices_count = 100;

	for (int i = 0; i < vertices_count; i++) {
		float ang = glm::radians(360.0f / vertices_count * i);
		float coss = glm::cos(ang);
		float sinn = glm::sin(ang);
		vertices[i * 4 + 0] = coss;
		vertices[i * 4 + 1] = sinn;
		vertices[i * 4 + 2] = 0; // u
		vertices[i * 4 + 3] = 0; // v
	}

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}