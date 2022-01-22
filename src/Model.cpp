#include "Model.h"
#include <assimp/postprocess.h>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

Model::Model(char* filename)
{
	loadModel(std::string(filename));
}

Model::~Model()
{

}

void Model::Draw()
{
	for (auto mesh : m_meshes)
	{
		mesh->Draw();
	}
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

	m_directory = dir.substr(0, dir.find_last_of('/'));

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
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex newVertex;

		aiVector3D currentAiVec = mesh->mVertices[i]; //aktualnie przetwarzamy wektor; musimy odpowiednio przepisac wartosci, bo ni ma bezposredniej konwersji
		glm::vec3 vec(currentAiVec.x, currentAiVec.y, currentAiVec.z);

		newVertex.position = vec;

		currentAiVec = mesh -> mNormals[i];
		vec = glm::vec3(currentAiVec.x, currentAiVec.y, currentAiVec.z);
		newVertex.normal = vec;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			newVertex.textureCoord = vec;
		}
		else
		{
			newVertex.textureCoord = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(newVertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return new Mesh(vertices, indices);

}
