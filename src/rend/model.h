#pragma once

#include "shader.h"
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

struct Model {
	Model(std::string path);
	void draw(Shader shader);
private:
	std::vector<Mesh> meshes;
	void load(std::string path);

private:
	std::string directory;
	void process_node(aiNode* node, const aiScene* scene);
	Mesh process_mesh(aiMesh* node, const aiScene* scene);
	std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type);
};