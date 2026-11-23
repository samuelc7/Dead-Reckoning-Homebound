#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <glad/glad.h> // include glad to get the required OpenGL headers

#include "shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture_struct
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	// mesh data
	std::vector<Vertex>	vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture_struct> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture_struct> textures);
	void Draw(Shader& shader);

private:
	// render data
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif // !MESH_H