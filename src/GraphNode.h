#pragma once
#include "Model.h"\
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>



class GraphNode
{
private:
    bool dirty;

    std::vector<GraphNode*> children;

    glm::mat4* matrix;
    glm::mat4 world = glm::mat4(1.0f);
    glm::mat4 local = glm::mat4(1.0f);

public:


    GraphNode(GraphNode* parent = NULL, glm::mat4* matrix = NULL, glm::mat4 local = glm::mat4(1))
    {
        this->matrix = matrix;
        if (matrix != NULL) {
            *this->matrix = local;
        }
        this->local = local;
        this->world = this->local;
        //poniewaz jeszcze nie przeliczyli?my transformacji world
        this->dirty = true;
        if (parent != NULL) {
            addParent(parent);
        }
    }

    void Update(glm::mat4 parentTransform = glm::mat4(1.0), bool dirtyP = false)
    {
        //operator OR, true je?eli "flaga rodzica" lub "flaga" posiada wartosc true
        dirtyP |= dirty;

        //je?eli dirty == false pomijamy przeliczanie transformacji world
        if (dirtyP)
        {
            world = parentTransform * local;
            if (matrix != NULL) {
                *matrix = world;
            }
            dirty = false;
        }


        for (auto child : children)
        {
            child->Update(world, dirtyP);
        }
    }

    std::vector< GraphNode*> getChildren() {
        return children;
    }

    void setTransform(glm::mat4 local)
    {
        this->local = local;
        dirty = true;
    }

    //    void setTransform(glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation){
    //        setTranslation(translation);
    //        setScale(scale);
    //        setRotation(rotation);
    //    }

    void Translate(glm::vec3 translation) {
        this->local = glm::translate(local, translation);
        dirty = true;
    }

    void Translate(float x, float y, float z) {
        this->local = glm::translate(local, glm::vec3(x, y, z));
        dirty = true;
    }

    void Scale(glm::vec3 scale) {
        this->local = glm::scale(local, scale);
        dirty = true;
    }

    void Scale(float x, float y, float z) {
        this->local = glm::scale(local, glm::vec3(x, y, z));
        dirty = true;
    }

    void Rotate(glm::vec3 rotation) {
        this->local = glm::rotate(local, rotation.x, glm::vec3(1.0, 0.0, 0.0));
        this->local = glm::rotate(local, rotation.y, glm::vec3(0.0, 1.0, 0.0));
        this->local = glm::rotate(local, rotation.z, glm::vec3(0.0, 0.0, 1.0));
        dirty = true;
    }

    void Rotate(float x, float y, float z) {
        this->local = glm::rotate(local, x, glm::vec3(1.0, 0.0, 0.0));
        this->local = glm::rotate(local, y, glm::vec3(0.0, 1.0, 0.0));
        this->local = glm::rotate(local, z, glm::vec3(0.0, 0.0, 1.0));
        dirty = true;
    }

    void addChild(GraphNode* child)
    {
        children.push_back(child);
        dirty = true;
    }

    void addParent(GraphNode* parent) {
        parent->addChild(this);
        dirty = true;
    }

    glm::mat4 getWorldTransform() {
        return world;
    }


};
