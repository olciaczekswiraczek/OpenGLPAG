#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <vector>
#include <string>

class Light 
{ 
public:
	Light();
	~Light();

	void InitLightObject();

private:
	std::string type;

	GLuint VAO;
	GLuint VBO;




};