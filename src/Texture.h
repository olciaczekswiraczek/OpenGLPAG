#pragma once
#include <glad/glad.h>
#include <string>



class Texture
{
public:
	Texture(const char* filename, const char* type);
	~Texture();

	GLuint getTexture();
	std::string getPath();

	void Use(int id);

private:
	bool Load(const char* filename);
	GLuint texture;

	std::string m_type;
	std::string path;

};