#include "Texture.h"
#include <stb_image.h>
#include <iostream>

Texture::Texture(const char* filename, const char* type)
    :m_type(type), path(filename)
{
    Load(filename);
}

Texture::~Texture()
{
}

GLuint Texture::getTexture()
{
    return texture;
}

std::string Texture::getPath()
{
    return path;
}

void Texture::Use(int id)
{
    glActiveTexture(GL_TEXTURE0 + id); //GL_TEXTUREn w gladzie s¹ kolejno st¹d dodajemy id
    glBindTexture(GL_TEXTURE_2D, texture);
}

bool Texture::Load(const char* filename)
{
    // load and create a texture 
  // -------------------------

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters, MAG - du¿e tekstury
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL-LINEAR - "mieszanka" kolorów najbli¿szych pikseli
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else
    {
        GLuint error = glGetError();
        std::cerr << "Failed to load texture: " << error << std::endl;
        return false;
    }
    stbi_image_free(data);
    return true;


}
