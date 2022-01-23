#pragma once
#include "Mesh.h"
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class Model
{

public:
	Model(char *filename);
	~Model();

	void Draw();

private:

	std::vector<Mesh*> m_meshes;
	std::string m_directory;

	void loadModel(std::string& dir);

	void processNode(aiNode* node, const aiScene* scene);

	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	


};