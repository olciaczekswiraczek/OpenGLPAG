#include "Model.h"
#include <assimp/postprocess.h>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

Model::Model()
{

}

Model::~Model()
{

}

void Model::loadModel(std::string& dir)
{

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(dir, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !(scene->mRootNode)) // scena nieza³adowana, scena niekompletna, nie mamy root node'a
	{
		std::cerr << "Error loading model" << importer.GetErrorString() << "\n";
		return;
	}

	m_directory = dir.substr(0, dir.find_last_of("/\\"));

	processNode(scene->mRootNode, scene);
	
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		this->m_meshes.push_back(processMesh(mesh,scene));
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}


}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) //dodaje meshe
{
	return nullptr;
}
