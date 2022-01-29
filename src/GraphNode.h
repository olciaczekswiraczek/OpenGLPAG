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

	virtual void Draw();
	virtual void Update();

	void addChild(GraphNode* node);

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