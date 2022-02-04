#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <vector>
#include <string>

#include "ShaderProgram.h"

class Light
{
public:
	Light(std::vector<float>& vertices);
	~Light();

	GLuint getLightObjectVAO();
	GLuint getVBO();

	void InitLightObject();
	void drawLightObject(ShaderProgram* shaderProgram, glm::mat4* model);

private:
	
	GLuint lightObjectVAO;
	GLuint VBO;

	std::vector<float>& vertices;
	std::string type;




};