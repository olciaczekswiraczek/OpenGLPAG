#include "Model.h"
#include <assimp/postprocess.h>
#include <iostream>
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

Model::Model(char* filename, ShaderProgram* shaderProgram):
	shaderProgram(shaderProgram)
{
	loadModel(std::string(filename));
	transform = new glm::mat4(1);
}

Model::~Model()
{
	for (auto mesh : m_meshes)
	{
		delete mesh;
	}

	
}

void Model::Draw()
{
	for (auto mesh : m_meshes)
	{
		mesh->Draw(shaderProgram, transform);
	}
}

void Model::setTransform(glm::mat4* matrix)
{
	transform = matrix;
}

void Model::setShaderProgram(ShaderProgram* s)
{
	shaderProgram = s;
}


void Model::loadModel(std::string& dir)
{

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(dir, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !(scene->mRootNode)) // scena niezaładowana, scena niekompletna, nie mamy root node'a
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
	std::vector<Texture*> textures;

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

	
	/* if (mesh->mMaterialIndex >= 0)
	 {
		 aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];//pobieramy material o odpowiednim indeksie
		 newMesh->loadMaterialTexture(mat, aiTextureType_DIFFUSE, "texture_diffuse");
	 } */

	 // przetwórz materia³y
	 if (mesh->mMaterialIndex >= 0)
	 {
		 aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		 std::vector<Texture*> diffuseMaps = loadMaterialTextures(material,
			 aiTextureType_DIFFUSE, "texture_diffuse");
		 textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		 std::vector<Texture*> specularMaps = loadMaterialTextures(material,
			 aiTextureType_SPECULAR, "texture_specular");
		 textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	 }

	 return new Mesh(vertices, indices, textures);
}

std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString name;
		mat->GetTexture(type, i, &name);

		bool skip = false;

		for (auto loadedTexture : textures_loaded) //sprawdzamy czy nie mamy tesktury o danej nazwie, czy nie jest rowna aktualnie przetwarzanej sciezce/nazwie
		{
			if (loadedTexture->getPath() == std::string(name.C_Str()))
			{
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // jeœli tekstura nie zosta³a jeszcze za³adowana, za³aduj j¹
			Texture* texture = TextureFromFile(name.C_Str(), m_directory);
			
			textures.push_back(texture);
			textures_loaded.push_back(texture); // dodaj do za³adowanych wektora textures_loaded
		}
	}
	return textures;
}
Texture* Model::TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename0 = std::string(path);
	filename0 = directory + '/' + filename0;
	const char* filename = filename0.c_str();

	std::cout << "Halooooooo " << filename << '\n';

	Texture* tex = new Texture(filename, "texture_diffuse");

	return tex;
}


