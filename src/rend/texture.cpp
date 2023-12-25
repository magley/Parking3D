#include "texture.h"
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"


Texture::Texture(const char* fname) {
	int x, y, n;
	unsigned char *data = stbi_load(fname, &x, &y, &n, 0);
	if (data == nullptr) {
		printf("Failed loading image %s\n", fname);
		exit(1);
	}
	w = x;
	h = y;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}