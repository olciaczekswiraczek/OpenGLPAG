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

	void generateCone(int num_stacks, float height, float radius) {
		
		const float pi = 3.1415926535f;
		
		//std::vector<unsigned int> indices;
		//std::vector<float> vertices;
		if (num_stacks >= 3)
		{
			float angle = 2 * pi / num_stacks;
			unsigned int num_vertices = num_stacks + 2;
			unsigned int num_indices = num_stacks * 2 * 3;

			glm::vec3* vertices = new glm::vec3[num_vertices];
			GLuint* indices = new GLuint[num_indices];

			//CMeshVertex* vertices = new CMeshVertex[num_vertices];
			//GLuint* indices = new GLuint[num_indices];

			vertices[0] = glm::vec3{ 0,0,0 };
			//vertices[0].color = { 1.0f,0.0f,0.0f,0.0f };
			//vertices[1].color = { 0.0f,1.0f,0.0f,0.0f };

			vertices[num_vertices - 1] = glm::vec3{ 0,0,height }; //tip

			int num_angle = 1;
			for (int i = 0; i < num_stacks; ++i) {
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
					indices[1] = num_stacks;
					indices[2] = 1;
					indices[3] = num_vertices - 1;
					indices[4] = num_stacks;
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


	void generateOrbit(int num_segments, int num_rings, float thickness, float R) {
		std::vector<unsigned int> indexBuffer;
		std::vector<float> vertBuffer;
		int num_vertices = (num_rings + 1) * (num_segments + 1);

		const float pi = 3.1415926535f;
		const float r1 = R;
		const float r2 = thickness;
		for (int i = 0, index = 0; i <= num_rings; ++i) {
			for (int j = 0; j <= num_segments; ++j, ++index) {
				float u = float(i) / num_rings;
				float v = (float(j) + u) / num_segments;

				// Compute angles
				float u_angle = u * 2 * pi;
				float v_angle = v * 2 * pi;

				// Position
				float x = cos(u_angle) * (r1 + cos(v_angle) * r2);
				float y = sin(u_angle) * (r1 + cos(v_angle) * r2);
				float z = sin(v_angle) * r2;
				vertBuffer.push_back(x);
				vertBuffer.push_back(y);
				vertBuffer.push_back(z);
			}
		}

		// Compute torus indices
		for (int i = 0, index = 0; i <= num_vertices; ++i) {
			indexBuffer.push_back(int(i % num_vertices));
			indexBuffer.push_back(int((i + num_segments + 1) % num_vertices));
		}
		vert = vertBuffer;
		m_elementBuffer = indexBuffer;
		setupMesh2();
	}

private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	std::vector<float> vert; //do stozka

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

