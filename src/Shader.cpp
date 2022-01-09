#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>


Shader::Shader(const char* filename, ShaderType sType)
	: shaderType(sType), filename(filename)
{
	LoadFromFile(filename, sType);
}


Shader::~Shader()
{
}

GLint Shader::getShaderID()
{
	return shaderID;
}

void Shader::Refresh()
{
	LoadFromFile(filename.c_str(), shaderType);
}

void Shader::LoadFromFile(const char* filename, ShaderType shaderType)
{
	std::fstream file;
	file.open(filename, std::ios::in);

	if (!file.good())
	{
		std::perror("Error opening file: ");
		return;
	}

	std::stringstream sstream;
	sstream << file.rdbuf();

	file.close();

	std::string str = sstream.str();

	const char* shaderText = str.c_str(); //nasz teskt
	std::cout << shaderText;

	GLint success;
	GLchar errBuffer[512];

	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderText, NULL); //shaderText
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, errBuffer);
		std::cerr << "Failed to compile shader from file: " + std::string(filename) + " " + errBuffer;
	}
}
