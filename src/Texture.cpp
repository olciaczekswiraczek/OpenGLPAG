#include "Texture.h"
#include <stb_image.h>
#include <iostream>

Texture::Texture(const char* filename, const char* type)
    :type(type), path(filename)
{
    Load(filename);
}

Texture::~Texture()
{
}

GLuint Texture::getTextureID()
{
    return textureID;
}

std::string Texture::getType()
{
    return type;
}

std::string Texture::getPath()
{
    return path;
}

void Texture::Use(int id)
{
    glActiveTexture(GL_TEXTURE0 + id); //GL_TEXTUREn w gladzie sa kolejno stad dodajemy id
    glBindTexture(GL_TEXTURE_2D, textureID);
}

bool Texture::Load(const char* filename)
{
    // load and create a texture 
  // -------------------------
 
   
    glGenTextures(1, &textureID);
    std::cout << " Ehs: " << textureID;

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    
    return true;


}
