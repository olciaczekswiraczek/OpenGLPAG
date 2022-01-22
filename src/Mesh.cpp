#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& elementBuffer)
	:m_vertexBuffer(vertexBuffer),m_elementBuffer(elementBuffer)
{
	Init();
}



Mesh::~Mesh()
{
	//delete vertices;
	//delete elementBuffer;
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

GLuint Mesh::getVAO()
{
	return VAO;
}



void Mesh::Draw()
{

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_vertexBuffer.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(NULL);
	//glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, NULL); //count liczba indicies, offset
	
}

void Mesh::Init()
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferDataV(GL_ARRAY_BUFFER, m_vertexBuffer, GL_STATIC_DRAW);

	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferDataV(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer, GL_STATIC_DRAW);

	//pozycja
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position)); //indeksy, liczba elementow
	glEnableVertexAttribArray(0);

	//tex coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoord));
	glEnableVertexAttribArray(1);

	//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(NULL); // odpiecie vao
}

