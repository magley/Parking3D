#pragma once

struct Texture {
	unsigned tex = -1;
	unsigned w = 0;
	unsigned h = 0;

	Texture(const char* fname);
};