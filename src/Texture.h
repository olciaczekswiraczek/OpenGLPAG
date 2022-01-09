#pragma once
#include <glad/glad.h>

class Texture
{
public:
	Texture(const char* filename);
	~Texture();

	GLuint getTexture();

	void Use(int id);

private:
	bool Load(const char* filename);
	GLuint texture;

};