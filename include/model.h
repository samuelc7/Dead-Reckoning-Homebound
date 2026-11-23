#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "texture.h"

class Model
{
public:
	Model(std::string path)
	{
		loadModel(path);
	}
	void Draw(Shader& shader);
private:
	// model data
	std::vector<Mesh> meshes;
	std::vector<Texture_struct> textures_loaded;
	std::string directory;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture_struct> loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char *path, std::string directory);
};

#endif // !MODEL_H
