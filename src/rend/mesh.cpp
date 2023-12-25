#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::vector<Texture>& textures) {
	setup_mesh(vertices, indices, textures);
}

void Mesh::setup_mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::vector<Texture>& textures) {
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	this->textures = textures;
	indices_size = indices.size();
}

float ang = 0.0f;
bool rise = true;

void _temp_ramp_anim() {
	if (rise) {
		ang += 0.1f;
		if (ang >= 95) {
			rise = false;
		}
	}
	else {
		ang -= 0.1f;
		if (ang <= 0) {
			rise = true;
		}
	}
}

void Mesh::draw(const Shader& shd) {
	// TODO: Externalize model matrix for each entity (and mesh?).
	glm::mat4 model = glm::mat4(1.0);
	_temp_ramp_anim();
	model = glm::rotate(model, glm::radians(ang), glm::vec3(1.0, 0.0, 0.0));

	shd.set_mat4("M", &model[0][0]);

	glUseProgram(shd.prog);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindVertexArray(vao);

	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].tex);
	}

	glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
}