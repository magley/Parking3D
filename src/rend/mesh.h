#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "texture.h"
#include "shader.h"
#include "material.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
};

struct Mesh {
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, Material material);
	void draw(const Shader* shd);

	unsigned vbo;
	unsigned vao;
	unsigned ebo;
	unsigned indices_size;
	Material material;

	void setup_mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);
};