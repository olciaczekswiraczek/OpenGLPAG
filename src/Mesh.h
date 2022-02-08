#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <vector>
#include <sstream>

#include "Texture.h"
#include <assimp/material.h>
#include "ShaderProgram.h"


#define PI 3.14159265358979324

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
	Mesh(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& elementBuffer, std::vector<Texture*> textures);
	Mesh()
	{

	}
	~Mesh();

	GLuint getVAO();

	void Draw(ShaderProgram * shaderProgram, glm::mat4* model, bool& isFromFile);

	void loadMaterialTexture(aiMaterial* material, aiTextureType textureType, const char* typeName);

	void generateCone(int num_segments, float height, float radius) {
		
			
		//std::vector<unsigned int> indices;
		//std::vector<float> vertices;
		if (num_segments >= 3)
		{
			float angle = 2 * PI / num_segments;
			unsigned int num_vertices = num_segments + 2;
			unsigned int num_indices = num_segments * 2 * 3;

			glm::vec3* vertices = new glm::vec3[num_vertices];
			GLuint* indices = new GLuint[num_indices];

			//CMeshVertex* vertices = new CMeshVertex[num_vertices];
			//GLuint* indices = new GLuint[num_indices];

			vertices[0] = glm::vec3{ 0,0,0 };
			//vertices[0].color = { 1.0f,0.0f,0.0f,0.0f };
			//vertices[1].color = { 0.0f,1.0f,0.0f,0.0f };

			vertices[num_vertices - 1] = glm::vec3{ 0,0,height }; //tip

			int num_angle = 1;
			for (int i = 0; i < num_segments; ++i) {
				glm::vec3 vert = glm::vec3(cos(angle * i) * radius, -sin(angle * i) * radius, 0.0f);
				vertices[i + 1].x = cos(angle * i) * radius;
				vertices[i + 1].y = -sin(angle * i) * radius;
				vertices[i + 1].z = 0;
				//vertices[i + 1] = vert;


				if (i > 0) {
					indices[6 * i] = 0;
					indices[6 * i + 1] = i;
					indices[6 * i + 2] = i + 1;
					indices[6 * i + 3] = num_vertices - 1;
					indices[6 * i + 4] = i;
					indices[6 * i + 5] = i + 1;
				}
				else if (i == 0)
				{
					indices[0] = 0;
					indices[1] = num_segments;
					indices[2] = 1;
					indices[3] = num_vertices - 1;
					indices[4] = num_segments;
					indices[5] = 1;
				}
			}

			//CreateGLResources(vertices, indices);

			std::vector<unsigned int> indexBuffer;
			std::vector<float> vertBuffer;

			for (int i = 0; i < num_vertices; i++)
			{
				vertBuffer.push_back(vertices[i].x);
				vertBuffer.push_back(vertices[i].y);
				vertBuffer.push_back(vertices[i].z);
			}
			for (int i = 0; i < num_indices; i++)
			{
				indexBuffer.push_back(indices[i]);
			}

			vert = vertBuffer;
			m_elementBuffer = indexBuffer;
			setupMesh2();

			delete[] vertices;
			delete[] indices;
		}
		
	}


	void generateOrbit(int num_divisions, float origin[3], float radius) {
		std::vector<unsigned int> indexBuffer;
		std::vector<float> vertBuffer;
		

		for (int j = 0; j <= num_divisions; j ++)
		{

			// Position
			float x = origin[0] + radius * cos((float)j / num_divisions * 2 * PI);
			float y = origin[1] + radius * sin((float)j / num_divisions * 2 * PI);
			float z = origin[2] + 0;

			vertBuffer.push_back(x);
			vertBuffer.push_back(y);
			vertBuffer.push_back(z);
		}
		
		vert = vertBuffer; 
		setupMesh2();
	}



private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	std::vector<float> vert; //do stozka i orbit

	std::vector<Vertex> m_vertexBuffer;
	std::vector<GLuint> m_elementBuffer;

	std::vector <Texture*> m_textures;

	void Init();
	void Init2();

	void setupMesh2()
	{
		// create buffers/arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		if (m_elementBuffer.size() != 0)
			glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);

		if (m_elementBuffer.size() != 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferDataV(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer, GL_STATIC_DRAW);
		}
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindVertexArray(0);
	}


};

