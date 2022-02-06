#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& elementBuffer)
	:m_vertexBuffer(vertexBuffer),m_elementBuffer(elementBuffer)
{
	Init();
}

Mesh::Mesh(std::vector<Vertex>& vertexBuffer, std::vector<GLuint>& elementBuffer, std::vector<Texture*> textures)
	: m_vertexBuffer(vertexBuffer), m_elementBuffer(elementBuffer), m_textures(textures)
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



void Mesh::Draw(ShaderProgram* shaderProgram, glm::mat4* model, bool& isFromFile)
{
	//shaderProgram->Use();
	//shaderProgram->setMat4("model" ,*model);
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = m_textures.at(i)->getType();
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shaderProgram->getProgramID(), (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, m_textures.at(i)->getTextureID());
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_elementBuffer.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);

	
}
/*
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
			Texture* texture = new Texture(name.C_Str(), typeName);
			
			m_textures.push_back(texture);
		}


	}
}*/

void Mesh::Init()
{

	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, m_vertexBuffer.size() * sizeof(Vertex), &m_vertexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer.size() * sizeof(unsigned int), &m_elementBuffer[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	
	glBindVertexArray(0);
}

void Mesh::Init2()
{
	
}



