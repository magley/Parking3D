#include "mesh2d.h"
#include "GL/glew.h"

Mesh2D::Mesh2D(Mesh2D::Type type) {
	switch (type) {
	case SQUARE:
		init_square();
		return;
	}
}

void Mesh2D::draw(const Shader* shd, Texture* texture) {
	glUseProgram(shd->prog);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture->tex);
	glDrawArrays(GL_TRIANGLES, 0, vertices_count);
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