#include "Light.h"

Light::Light(std::vector<float>& vertices):
    vertices(vertices)
{
    InitLightObject();
}
GLuint Light::getVBO()
{
    return VBO;
}
void Light::InitLightObject()
{
    // configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &lightObjectVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(lightObjectVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void Light::drawLightObject(ShaderProgram* shaderProgram, glm::mat4* model)
{
    shaderProgram->Use();
    shaderProgram->setMat4("model", *model);

    glBindVertexArray(lightObjectVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
