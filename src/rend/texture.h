#pragma once

struct Texture {
	unsigned tex = -1;
	unsigned w = 0;
	unsigned h = 0;
	unsigned fullw = 0;
	unsigned fullh = 0;

	Texture(unsigned tex, unsigned w, unsigned h);
	Texture(const char* fname, int frames = 1);
};