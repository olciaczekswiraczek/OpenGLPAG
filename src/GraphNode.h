#pragma once
#include "Model.h"\
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Transform
{

};


class GraphNode
{
public:
	GraphNode(Model* model = nullptr);
	~GraphNode();

	void setTransform(glm::mat4* matrix);
	glm::mat4* getTransform();

	// update transforms
	// -----------------
	virtual void Update();

	// draw whole scene
	// ----------------
	virtual void Draw();
	

	void addChild(GraphNode* node);
	void addOrbit(float radius, ShaderProgram* shaderProgram, float thickness, float upTransform);

	void Translate(glm::vec3 translation);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(glm::vec3 scale);

protected:
	Model* model;
	GraphNode* parent;
	bool dirty;

	glm::mat4* worldTransform;
	glm::mat4* transform;

	std::vector<GraphNode*> children;


};