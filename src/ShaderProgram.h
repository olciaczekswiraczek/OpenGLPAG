#pragma once
#include <glad/glad.h>
#include "Shader.h"
#include <memory>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_float4x4.hpp>

class ShaderProgram
{
public:
	ShaderProgram(std::shared_ptr<Shader> vs, std::shared_ptr<Shader> fs);
	~ShaderProgram();

	static ShaderProgram* currentProgram;

	GLuint getProgramID();

	void Use();
	void Refresh();

	void setColor(glm::vec4 color, const std::string& name);
	void setMat4(glm::mat4& matrix, const std::string& name);


private:
	GLint programID;
	std::shared_ptr<Shader> vertexShader;
	std::shared_ptr<Shader> fragmentShader;

	void Init(bool mustCreate = true);

};

