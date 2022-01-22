#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <vector>


template <typename T>
void glBufferDataV(GLuint target, const std::vector<T>& buffer, GLenum type)
{
	glBufferData(target, sizeof(T) * buffer.size(), &buffer[0], type);
}

struct Vertex
{
	glm::vec3 position;
	glm::vec2 textureCoord;
};

class Mesh
{
public:
	Mesh(float x, float y, float z, float edgeLength);
	//VertexArrayObject(GLfloat d, glm::vec3 centerPos);
	~Mesh();

	GLuint getVAO();
	void Draw();

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	float x, y, z;

	float edgeLength;

	//std::vector<Vertex>* vertexBuffer;
	//std::vector<GLuint>* elementBuffer;

	void Init();





};

