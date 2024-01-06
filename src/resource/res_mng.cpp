#pragma once
#include "res_mng.h"

#include "util/log.h"

Texture* ResMng::load_tex(const std::string& fname, int frames) {
	const std::string path = "assets/texture/" + fname;
	auto it = tex.find(fname);

	if (it == tex.cend()) {
		log("Loading: " + path);
		Texture* t = new Texture(path.c_str(), frames);
		tex[fname] = t;
		it = tex.find(fname);
	}

	return it->second;
}

Shader* ResMng::load_shd(const std::string& fname) {
	const std::string path = "assets/shader/" + fname;
	auto it = shd.find(fname);

	if (it == shd.cend()) {
		log("Loading: " + path);
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
		log("Loading: " + path);
		Model* m = new Model(path.c_str());
		mdl[fname] = m;
		it = mdl.find(fname);
	}

	return it->second;
}

WavSample* ResMng::load_wav(const std::string& fname) {
	const std::string path = "assets/sound/" + fname;
	auto it = wav.find(fname);

	if (it == wav.cend()) {
		log("Loading: " + path);
		WavSample* m = new WavSample(path.c_str());
		wav[fname] = m;
		it = wav.find(fname);
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
	for (auto& it : wav) {
		free(it.second);
	}

	tex.clear();
	shd.clear();
	mdl.clear();
	wav.clear();
}
