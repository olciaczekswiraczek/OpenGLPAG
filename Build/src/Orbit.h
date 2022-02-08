#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shaderProgram.h>




class Orbit
{


public:
	

	Orbit(Shader s, float radiusP, glm::vec3 colorP)
	{
		this->radius = radiusP;
		this->color = colorP;
		Init();
	}

	void Draw()
	{
		shaderProgram->setInt("radius", radius);
		shaderProgram->setVec3("color", color);
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 1);
	}

	void Init()
	{

		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(origin), &origin, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		glBindVertexArray(0);

	}

private:
	GLuint VAO, VBO;
	ShaderProgram* shaderProgram;
	GLuint texture;
	glm::vec3 color;
	float radius;
	float origin[3] = {
		0.0f, 0.0f, 0.0f
	};
};