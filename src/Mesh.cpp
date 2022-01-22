#include "Mesh.h"

Mesh::Mesh(float x, float y, float z, float edgeLength) : x(x), y(y), z(z), edgeLength(edgeLength)
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
	//glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, NULL); //count liczba indicies, offset
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(NULL);
}

void Mesh::Init()
{
	float a = edgeLength / 2;

	float vertices[] = {
	   x - a, y - a, z - a,  0.0f, 0.0f,
	   x + a, y - a, z - a,  1.0f, 0.0f,
	   x + a, y + a, z - a,  1.0f, 1.0f,
	   x + a, y + a, z - a,  1.0f, 1.0f,
	   x - a, y + a, z - a,  0.0f, 1.0f,
	   x - a, y - a, z - a,  0.0f, 0.0f,

		x - a, y - a, z + a,  0.0f, 0.0f,
		x + a, y - a, z + a,  1.0f, 0.0f,
		x + a, y + a, z + a,  1.0f, 1.0f,
		x + a, y + a, z + a,  1.0f, 1.0f,
		x - a, y + a, z + a,  0.0f, 1.0f,
		x - a, y - a, z + a,  0.0f, 0.0f,

		x - a, y + a, z + a,  1.0f, 0.0f,
		x - a, y + a, z - a,  1.0f, 1.0f,
		x - a, y - a, z - a,  0.0f, 1.0f,
		x - a, y - a, z - a,  0.0f, 1.0f,
		x - a, y - a, z + a,  0.0f, 0.0f,
		x - a, y + a, z + a,  1.0f, 0.0f,

		x + a, y + a, z + a,  1.0f, 0.0f,
		x + a, y + a, z - a,  1.0f, 1.0f,
		x + a, y - a, z - a,  0.0f, 1.0f,
		x + a, y - a, z - a,  0.0f, 1.0f,
		x + a, y - a, z + a,  0.0f, 0.0f,
		x + a, y + a, z + a,  1.0f, 0.0f,

		x - a, y - a, z - a,  0.0f, 1.0f,
		x + a, y - a, z - a,  1.0f, 1.0f,
		x + a, y - a, z + a,  1.0f, 0.0f,
		x + a, y - a, z + a,  1.0f, 0.0f,
		x - a, y - a, z + a,  0.0f, 0.0f,
		x - a, y - a, z - a,  0.0f, 1.0f,

		x - a, y + a, z - a,  0.0f, 1.0f,
		x + a, y + a, z - a,  1.0f, 1.0f,
		x + a, y + a, z + a,  1.0f, 0.0f,
		x + a, y + a, z + a,  1.0f, 0.0f,
		x - a, y + a, z + a,  0.0f, 0.0f,
		x - a, y + a, z - a,  0.0f, 1.0f


	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBufferDataV(GL_ARRAY_BUFFER, *vertexBuffer, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferDataV(GL_ELEMENT_ARRAY_BUFFER, *elementBuffer, GL_STATIC_DRAW);


	//pozycja
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); //ustawia wskazniki na atrybuty na naszym VBO, pozycja 0, posiada 3 glfloaty, wyrównanie, rozmiar elementu, offset 
	glEnableVertexAttribArray(0);  //enable 0 <-indeks

	//tekstura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),  (void*)offsetof(Vertex, textureCoord));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind buffor


	glBindVertexArray(NULL); //odpiecie VAO
}


/*
 * -a, -a, -a,  0.0f, 0.0f,
	   +a, -a, -a,  1.0f, 0.0f,
	   +a, +a, -a,  1.0f, 1.0f,
	   +a, +a, -a,  1.0f, 1.0f,
	   -a, +a, -a,  0.0f, 1.0f,
	   -a, -a, -a,  0.0f, 0.0f,

		-a, -a, +a,  0.0f, 0.0f,
		+a, -a, +a,  1.0f, 0.0f,
		+a, +a, +a,  1.0f, 1.0f,
		+a, +a, +a,  1.0f, 1.0f,
		-a, +a, +a,  0.0f, 1.0f,
		-a, -a, +a,  0.0f, 0.0f,

		-a, +a, +a,  1.0f, 0.0f,
		-a, +a, -a,  1.0f, 1.0f,
		-a, -a, -a,  0.0f, 1.0f,
		-a, -a, -a,  0.0f, 1.0f,
		-a, -a, +a,  0.0f, 0.0f,
		-a, +a, +a,  1.0f, 0.0f,

		+a, +a, +a,  1.0f, 0.0f,
		+a, +a, -a,  1.0f, 1.0f,
		+a, -a, -a,  0.0f, 1.0f,
		+a, -a, -a,  0.0f, 1.0f,
		+a, -a, +a,  0.0f, 0.0f,
		+a, +a, +a,  1.0f, 0.0f,

		-a, -a, -a,  0.0f, 1.0f,
		+a, -a, -a,  1.0f, 1.0f,
		+a, -a, +a,  1.0f, 0.0f,
		+a, -a, +a,  1.0f, 0.0f,
		-a, -a, +a,  0.0f, 0.0f,
		-a, -a, -a,  0.0f, 1.0f,

		-a, +a, -a,  0.0f, 1.0f,
		+a, +a, -a,  1.0f, 1.0f,
		+a, +a, +a,  1.0f, 0.0f,
		+a, +a, +a,  1.0f, 0.0f,
		-a, +a, +a,  0.0f, 0.0f,
		-a, +a, -a,  0.0f, 1.0f


 */