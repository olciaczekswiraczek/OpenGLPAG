#pragma once
#include "Model.h"\
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>



class GraphNode
{
public:
	GraphNode(Model* model = nullptr);
	~GraphNode();

	void setTransform(glm::mat4* matrix);
	glm::mat4 getTransform();

	// update transforms
	// -----------------
	virtual void Update()
	{
		if (parent) // if has a parent
		{
			*worldTransform = *parent->worldTransform * (*transform);
		}
		else //if does not have a parent
		{
			*worldTransform = *transform;
		}

		if (model) // if has a model
		{
			model->setTransform(worldTransform);
		}
		for (GraphNode* node : children)
		{
			node->Update();
		}

	}

	// draw whole scene
	// ----------------
	virtual void Draw()
	{
		if (model) { model->Draw(); }

		for (GraphNode* node : children)
		{
			node->Draw();
		}
	}

	void addChild(GraphNode* node);
	void addOrbit(float radius, ShaderProgram* shaderProgram, float thickness, float upTransform);

	void Translate(glm::vec3 translation);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(glm::vec3 scale);

protected:
	Model* model;
	GraphNode* parent;

	glm::mat4* worldTransform;
	glm::mat4* transform;

	std::vector<GraphNode*> children;


};