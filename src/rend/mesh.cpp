#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Mesh::Mesh(const std::vector<Vertex>& vertices, 
	const std::vector<unsigned>& indices,
	Material material): material(material) {
	setup_mesh(vertices, indices);
}

void Mesh::setup_mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices) {
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

	shd->set_int("material0.diffuse_map", 0);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, material.diffuse_map->tex);

	shd->set_int("material0.specular_map", 1);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, material.specular_map->tex);

	shd->set_int("material0.emission_map", 2);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, material.emission_map->tex);

	glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0);
}