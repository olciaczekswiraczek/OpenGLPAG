#include "ShaderProgram.h"

#include <iostream>

ShaderProgram* ShaderProgram::currentProgram = nullptr;

ShaderProgram::ShaderProgram(std::shared_ptr<Shader> vs, std::shared_ptr<Shader> fs) :vertexShader(vs), fragmentShader(fs)
{
	Init();
}

ShaderProgram::~ShaderProgram()
{
}

GLuint ShaderProgram::getProgram()
{
	return program;
}

void ShaderProgram::Use()
{
	glUseProgram(program); //aktywacja shaderów
	currentProgram = this;
}

void ShaderProgram::setColor(glm::vec4 color)
{
	GLuint location = glGetUniformLocation(program, "uniformColor");
	glUniform4fv(location, 1, (GLfloat*)&color);
}

void ShaderProgram::Refresh() //wczytujemy na nowo shadery z plików oraz inicjalizujemy VAO
{
	fragmentShader->Refresh();
	vertexShader->Refresh();
	Init();
}

void ShaderProgram::Init(bool mustCreate)
{
	if (mustCreate)
	{
		program = glCreateProgram(); //tworzymy program
	}
	glAttachShader(program, vertexShader->getShaderID());
	glAttachShader(program, fragmentShader->getShaderID());
	glLinkProgram(program);

	GLint success;
	GLchar errBuffer[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(program, 512, NULL, errBuffer);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errBuffer << std::endl;
	}
	glUseProgram(program);

	glUniform1i(glGetUniformLocation(program, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(program, "ourTexture1"), 1);

	glUseProgram(0);


}
