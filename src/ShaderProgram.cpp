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

GLuint ShaderProgram::getProgramID()
{
	return programID;
}

void ShaderProgram::Use()
{
	glUseProgram(programID); //aktywacja shaderów
	currentProgram = this;
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
		programID = glCreateProgram(); //tworzymy program
	}
	glAttachShader(programID, vertexShader->getShaderID());
	glAttachShader(programID, fragmentShader->getShaderID());
	glLinkProgram(programID);

	GLint success;
	GLchar errBuffer[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(programID, 512, NULL, errBuffer);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << errBuffer << std::endl;
	}
	glUseProgram(programID);

	glUniform1i(glGetUniformLocation(programID, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(programID, "ourTexture1"), 1);

	glUseProgram(0);


}


