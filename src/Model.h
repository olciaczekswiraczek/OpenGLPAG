#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{

public:
	Model(char *filename, ShaderProgram* shaderProgram);
	~Model();

	void Draw();

private:

	std::vector<Mesh*> m_meshes;
	std::string m_directory;

	ShaderProgram* shaderProgram;


	void setShader(ShaderProgram* newShaderProgram);
	void loadModel(std::string& dir);

	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	


};