#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <vector>
#include <string>

#include "ShaderProgram.h"

class Light
{
public:
	Light();
	~Light();

	void InitLightObject();
	void drawLightObject(ShaderProgram* shaderProgram, glm::mat4* model);

private:
	std::string type;

	GLuint VAO;
	GLuint VBO;




};