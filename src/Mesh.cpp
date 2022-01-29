#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& elementBuffer)
	:m_vertexBuffer(vertexBuffer),m_elementBuffer(elementBuffer)
{
	Init();
}



Mesh::~Mesh()
{
	//delete vertices;
	//delete elementBuffer;

	for (auto tex : m_textures)
	{
		delete tex;
	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

GLuint Mesh::getVAO()
{
	return VAO;
}



void Mesh::Draw(ShaderProgram* shaderProgram)
{

	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint heightNr = 1;

	for (GLuint i = 0; i < this->m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::stringstream ss;
		std::string number;
		std::string name = this->m_textures[i]->getType();

		if (name == "texture_diffuse")
			ss << diffuseNr++;
		else if (name == "texture_specular")
			ss << specularNr++;
		else if (name == "texture_normal")
			ss << normalNr++;
		else if (name == "texture_height")
			ss << heightNr++;
		number == ss.str();


		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shaderProgram->getProgramID(), (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, this->m_textures[i]->getTextureID());
	}


	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_vertexBuffer.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(NULL);
	//glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, NULL); //count liczba indicies, offset
	
}

void Mesh::loadMaterialTexture(aiMaterial* material, aiTextureType textureType, const char* typeName)
{

	int size = material->GetTextureCount(textureType); //ile jest teksur danego typu w tym materiale 

	for (int i = 0; i < size; i++)
	{
		aiString name;
		material->GetTexture(textureType, i, &name);

		bool skip = false;

		for (auto loadedTexture : m_textures) //sprawdzamy czy nie mamy tesktury o danej nazwie, czy nie jest rowna aktualnie przetwarzanej sciezce/nazwie
		{
			if (loadedTexture->getPath() == std::string(name.C_Str()))
			{
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture* newTexture = new Texture(name.C_Str(), typeName);
			m_textures.push_back(newTexture);
		}


	}
}

void Mesh::Init()
{

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferDataV(GL_ARRAY_BUFFER, m_vertexBuffer, GL_STATIC_DRAW);

	//
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferDataV(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer, GL_STATIC_DRAW);

	//pozycja
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position)); //indeksy, liczba elementow
	glEnableVertexAttribArray(0);

	//tex coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoord));
	glEnableVertexAttribArray(1);

	//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(NULL); // odpiecie vao
}

