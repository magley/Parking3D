#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh(const std::vector<Vertex>& vertices, 
	const std::vector<unsigned>& indices, 
	const std::vector<Texture*>& textures, 
	Material material): material(material) {
	setup_mesh(vertices, indices, textures);
}

void Mesh::setup_mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::vector<Texture*>& textures) {
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TexCoords)));
	glEnableVertexAttribArray(2);


	this->textures = textures;
	indices_size = indices.size();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::draw(const Shader* shd) {
	glUseProgram(shd->prog);
	glBindVertexArray(vao);

	shd->set_vec3("material0.ambient", material.ambient.r, material.ambient.g, material.ambient.b);
	shd->set_vec3("material0.diffuse", material.diffuse.r, material.diffuse.g, material.diffuse.b);
	shd->set_vec3("material0.specular", material.specular.r, material.specular.g, material.specular.b);
	shd->set_float("material0.shininess", material.shininess);

	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->tex);
	}

	glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
}