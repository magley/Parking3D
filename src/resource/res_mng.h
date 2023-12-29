#pragma once

#include <unordered_map>
#include "string.h"
#include "rend/texture.h"
#include "rend/shader.h"
#include "rend/model.h"

// TODO: Make filenames extension-agnostic.

struct ResMng {
	/// <summary>
	/// Borrow handle to a Texture resource. Loads it if not present.
	/// </summary>
	/// <param name="fname">If "x.png", ResMng will look for "assets/texture/x.png".</param>
	/// <param name="frames">Number of frames (horizontal).</param>
	/// <returns>Texture pointer</returns>
	Texture* load_tex(const std::string& fname, int frames = 1);
	
	/// <summary>
	/// Borrow handle to a Shader resource. Loads it if not present.
	/// </summary>
	/// <param name="fname">If "x", ResMng will look for "assets/shader/x.vert" and "assets/shader/x.frag".</param>
	/// <returns>Shader pointer</returns>
	Shader* load_shd(const std::string& fname);
	
	/// <summary>
	/// Borrow handle to a Model resource. Loads it if not present.
	/// 
	/// If the model requires other resources (like textures), the
	/// references must adhere to ResMng's public API. For example,
	/// textures should only contain the name of the file.
	/// </summary>
	/// <param name="fname">If "x.obj", ResMng will look for "assets/model/x.obj" and "assets/model/x.mtl".</param>
	/// <returns>Model pointer</returns>
	Model* load_mdl(const std::string& fname);
	
	void free_everything();
private:
	std::unordered_map<std::string, Texture*> tex;
	std::unordered_map<std::string, Shader*> shd;
	std::unordered_map<std::string, Model*> mdl;
};