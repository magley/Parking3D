#pragma once
#include "res_mng.h"

Texture* ResMng::load_tex(const std::string& fname) {
	const std::string path = "assets/texture/" + fname;
	auto it = tex.find(fname);

	if (it == tex.cend()) {
		Texture* t = new Texture(path.c_str());
		tex[fname] = t;
		it = tex.find(fname);
	}

	return it->second;
}

Shader* ResMng::load_shd(const std::string& fname) {
	const std::string path = "assets/shader/" + fname;
	auto it = shd.find(fname);

	if (it == shd.cend()) {
		Shader* s = new Shader((path + ".vert").c_str(), (path + ".frag").c_str());
		shd[fname] = s;
		it = shd.find(fname);
	}

	return it->second;
}

Model* ResMng::load_mdl(const std::string& fname) {
	const std::string path = "assets/model/" + fname;
	auto it = mdl.find(fname);

	if (it == mdl.cend()) {
		Model* m = new Model(path.c_str());
		mdl[fname] = m;
		it = mdl.find(fname);
	}

	return it->second;
}

void ResMng::free_everything() {
	for (auto& it : tex) {
		free(it.second);
	}
	for (auto& it : shd) {
		free(it.second);
	}
	for (auto& it : mdl) {
		free(it.second);
	}

	tex.clear();
	shd.clear();
	mdl.clear();
}
