#pragma once
#include "glad/glad.h"
#include <string>

enum ShaderType
{
	VERTEX_SHADER = GL_VERTEX_SHADER,
	FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};


class Shader
{
public:
	Shader(const char* filename, ShaderType shaderType);
	~Shader();

	GLint getShaderID();

	void Refresh();

private:
	void LoadFromFile(const char* filename, ShaderType shaderType);

	std::string filename;
	ShaderType shaderType;

	GLint shaderID;
};

