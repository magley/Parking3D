#include "model.h"
#include <stdio.h>
#include "global.h"

Model::Model(std::string path) {
    load(path);
}

void Model::draw(Shader* shader) {
    for (Mesh& m : meshes) {
        m.draw(shader);
    }
}


void Model::load(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        printf("Assimp error: %s\n", import.GetErrorString());
        exit(1);
    }

    directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene) {
    for (int i = 0; i < node->mNumMeshes; i++) {
        auto mesh = process_mesh(scene->mMeshes[node->mMeshes[i]], scene);
        meshes.push_back(mesh);
    }

    for (int i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    std::vector<Texture*> textures;

    for (int i = 0; i < mesh->mNumVertices; i++) {
        auto& v = mesh->mVertices[i];
        auto& n = mesh->mNormals[i];
        auto& uv = mesh->mTextureCoords[0][i];
        Vertex vert{};
        vert.Position = glm::vec3(v.x, v.y, v.z);
        vert.Normal = glm::vec3(n.x, n.y, n.z);
        vert.TexCoords = glm::vec2(uv.x, uv.y);

        vertices.push_back(vert);
    }

    for (int i = 0; i < mesh->mNumFaces; i++) {
        auto& f = mesh->mFaces[i];
        for (int j = 0; j < f.mNumIndices; j++) {
            indices.push_back(f.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        auto& mat = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture*> diffuseMaps = load_material_textures(mat, aiTextureType_DIFFUSE);
        for (int i = 0; i < diffuseMaps.size(); i++) {
            textures.push_back(diffuseMaps[i]);
        }
    }

    Mesh m(vertices, indices, textures);
    return m;
}

std::vector<Texture*> Model::load_material_textures(aiMaterial* mat, aiTextureType type) {
    std::vector<Texture*> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture* tex = glo::wctx.resmng.load_tex(std::string(str.C_Str()));
        textures.push_back(tex);
    }
    return textures;
}