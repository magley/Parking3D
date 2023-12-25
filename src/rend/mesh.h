#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "texture.h"
#include "shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Mesh {
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::vector<Texture*>& textures);
	void draw(const Shader* shd);
private:
	unsigned vbo;
	unsigned vao;
	unsigned ebo;
	unsigned indices_size;
	std::vector<Texture*> textures;
	void setup_mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::vector<Texture*>& textures);
};