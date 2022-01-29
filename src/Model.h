#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include <vector>

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

	glm::mat4* transform; 

	void Draw();
	void setTransform(glm::mat4* matrix);
	void setShaderProgram(ShaderProgram* s);

private:

	std::vector<Mesh*> m_meshes;
	std::string m_directory;

	ShaderProgram* shaderProgram;

	void loadModel(std::string& dir);

	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	


};