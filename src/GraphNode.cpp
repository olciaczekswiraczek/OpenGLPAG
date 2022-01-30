#include "GraphNode.h"

GraphNode::GraphNode(Model* model):
	model(model), parent(nullptr)

{
	transform = new glm::mat4(1);
	worldTransform = new glm::mat4(1);
}

GraphNode::~GraphNode()
{
	
}

void GraphNode::setTransform(glm::mat4* matrix)
{
	transform = matrix;
}

glm::mat4* GraphNode::getTransform()
{
	return transform;
}


void GraphNode::addChild(GraphNode* node)
{
	children.push_back(node);
	node->parent = this;
}

void GraphNode::addOrbit(float radius, ShaderProgram* shaderProgram, float thickness, float upTransform)
{
	Mesh* thorusMesh = new Mesh();
	thorusMesh->generateTorus(50, 45, thickness, radius);
	Model* thorusModel = new Model(thorusMesh);
	thorusModel->setShaderProgram(shaderProgram);
	GraphNode* pom = new GraphNode(thorusModel);
	glm::mat4* TransformNode1 = new glm::mat4(1);
	if (upTransform != 0) {

		*TransformNode1 = glm::translate(*(TransformNode1), glm::vec3(0.0f, upTransform, 0.0f));
	}

	pom->setTransform(TransformNode1);
	pom->Rotate(90, glm::vec3(1, 0, 0));
	addChild(pom);
}


void GraphNode::Translate(glm::vec3 translation)
{
	*transform = glm::translate(*transform, translation);
}

void GraphNode::Rotate(float angle, glm::vec3 axis)
{
	*transform = glm::rotate(*transform, glm::radians(angle), axis);
}

void GraphNode::Scale(glm::vec3 scale)
{
	*transform = glm::scale(*transform, scale);
}
