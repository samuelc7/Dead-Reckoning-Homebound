#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <iostream>
#include <glad/glad.h>

#include "stb_image.h"

class Texture
{
public:
	unsigned int ID;
	Texture(std::vector<std::pair<GLenum, GLenum>> option_value, const char *path, GLint internalformat);
};

#endif