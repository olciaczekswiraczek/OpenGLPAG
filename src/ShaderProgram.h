#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include <memory>
#include <glm/vec4.hpp>

class ShaderProgram
{
public:
	ShaderProgram(std::shared_ptr<Shader> vs, std::shared_ptr<Shader> fs);
	~ShaderProgram();

	static ShaderProgram* currentProgram;

	GLuint getProgramID();

	void Use();
	void setColor(glm::vec4 color);
	void Refresh();


private:
	GLint programID;
	std::shared_ptr<Shader> vertexShader;
	std::shared_ptr<Shader> fragmentShader;

	void Init(bool mustCreate = true);

};

