#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <vector>
#include <sstream>

#include "Texture.h"
#include <assimp/material.h>
#include "ShaderProgram.h"




template <typename T>
void glBufferDataV(GLuint target, const std::vector<T>& buffer, GLenum type)
{
	glBufferData(target, sizeof(T) * buffer.size(), &buffer[0], type);
}

struct Vertex
{
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;

};

class Mesh
{
public:
	Mesh(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& elementBuffer);
	Mesh(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& elementBuffer, std::vector<Texture*> textures);
	Mesh()
	{

	}
	~Mesh();

	GLuint getVAO();
	std::vector<GLuint> m_elementBuffer;

	void Draw(ShaderProgram * shaderProgram, glm::mat4* model, bool& isFromFile);

	void loadMaterialTexture(aiMaterial* material, aiTextureType textureType, const char* typeName);

	

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	std::vector<float> vert; //do stozka

	std::vector<Vertex> m_vertexBuffer;
	

	std::vector <Texture*> m_textures;

	void Init();
	void Init2();

	void setupMesh2()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferDataV(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);
	}


};

