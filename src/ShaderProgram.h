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


	void setInt(const std::string& name, int value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setColor(const std::string& name, glm::vec4 color) const;
	void setMat4(const std::string& name, glm::mat4& matrix) const;

	


private:
	GLint programID;
	std::shared_ptr<Shader> vertexShader;
	std::shared_ptr<Shader> fragmentShader;

	void Init(bool mustCreate = true);

};

