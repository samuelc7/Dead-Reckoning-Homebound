#include "texture.h"

Texture::Texture(std::vector<std::pair<GLenum, GLenum>> option_value, const char *filename, GLint internalformat)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// set the texture wrapping/filtering options (on currently bound texture)
	for (auto p : option_value)
	{
		glTexParameteri(GL_TEXTURE_2D, p.first, p.second);
	}

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, internalformat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture." << std::endl;
    }

    stbi_image_free(data);
}