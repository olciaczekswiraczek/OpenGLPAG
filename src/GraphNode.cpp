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

glm::mat4* GraphNode::getWorldTransform()
{
	return worldTransform;
}

std::vector<GraphNode*> GraphNode::getChildren()
{
	return children;
}

// update transforms
// -----------------
void GraphNode::Update()
{

	
	if (parent) // if has a parent
	{
		*worldTransform = *parent->worldTransform * (*transform);
	}
	else //if does not have a parent
	{
		*worldTransform = *transform;
	}

	//if (model) // if has a model
	//{
	//	model->setTransform(worldTransform);
	//}
	for (GraphNode* node : children)
	{
		node->Update();
	}
}


void GraphNode::Draw()
{
	{
		//if (model) { model->Draw(); }

		for (GraphNode* node : children)
		{
			node->Draw();
		}
	}
}

void GraphNode::addChild(GraphNode* node)
{
	children.push_back(node);
	node->parent = this;
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
