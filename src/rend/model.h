#pragma once

#include "shader.h"
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

struct Model {
	Model(std::string path);
	void draw(Shader* shader);

	std::vector<Mesh> meshes;
	void load(std::string path);

	std::string directory;
	void process_node(aiNode* node, const aiScene* scene);
	Mesh process_mesh(aiMesh* node, const aiScene* scene);
};