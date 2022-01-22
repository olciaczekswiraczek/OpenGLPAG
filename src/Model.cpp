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
	}

	m_directory = dir.substr(0, dir.find_last_of("/\\"));

	processNode(scene->mRootNode, scene);
	
}