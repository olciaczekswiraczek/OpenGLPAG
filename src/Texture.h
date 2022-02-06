#pragma once
#include <glad/glad.h>
#include <string>



class Texture
{
public:
	Texture(const char* filename, const char* type);
	Texture();
	~Texture();

	GLuint getTextureID();
	std::string getType();
	std::string getPath();
	

	void Use(int id);

private:
	bool Load(const char* filename);
	unsigned int textureID;

	std::string type;
	std::string path;

};