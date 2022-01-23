#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <vector>

#include "Texture.h"
#include <assimp/material.h>


template <typename T>
void glBufferDataV(GLuint target, const std::vector<T>& buffer, GLenum type)
{
	glBufferData(target, sizeof(T) * buffer.size(), &buffer[0], type);
}

struct Vertex
{
	glm::vec3 position;
	glm::vec2 textureCoord;
	glm::vec3 normal;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& elementBuffer);
	~Mesh();

	GLuint getVAO();

	void Draw();

	void loadMaterialTexture(aiMaterial* material, aiTextureType textureType, const char* typeName);

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	std::vector<Vertex> m_vertexBuffer;
	std::vector<GLuint> m_elementBuffer;

	std::vector <Texture*> m_textures;

	void Init();

	


};

