#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include <vector>
#include <string>

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{

public:
	Model(char *filename, ShaderProgram* shaderProgram = nullptr);
	Model(Mesh* mesh)
	{
		
		m_meshes.push_back(mesh);
		transform = new glm::mat4(1);
	}

	~Model();

	std::vector<Texture*> textures_loaded;
	std::vector<Mesh*> m_meshes;
	glm::mat4* transform; 

	void Draw();
	void setTransform(glm::mat4* matrix);
	void setShaderProgram(ShaderProgram* s);

	std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	


private:

	
	
	std::string m_directory;

	ShaderProgram* shaderProgram;
	Texture* TextureFromFile(const char* path, const std::string& directory, std::string typeName);

	bool isFromFile;

	void loadModel(std::string& dir);
	

	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	


};