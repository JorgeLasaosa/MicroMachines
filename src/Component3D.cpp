#include "Component3D.h"

#include <iostream>

Component3D::Component3D(Mesh3DRenderer* mesh, GLboolean zup)
    : position(glm::vec3(0,0,0)), rotation(glm::vec3(0,0,0)), scale(glm::vec3(1,1,1)), shear(0), parent(nullptr), mesh(mesh), zup(zup)
{
	this->mesh = mesh;
}

Component3D::~Component3D() {
}

void Component3D::setParent(Component3D* parent) {
	this->parent = parent;
	parent->childs.push_back(this);
}

void Component3D::setPosition(glm::vec3 position){
	this->position = position;
}

void Component3D::setRotation(glm::vec3 rotation){
	this->rotation = rotation;
}

void Component3D::setScale(glm::vec3 scale){
	this->scale = scale;
}

void Component3D::setShear(GLfloat shear){
	this->shear = shear;
}


glm::mat4 Component3D::getTransormMatrix(glm::mat4 model){
	if (parent != nullptr){
		model = parent->getTransormMatrix(model);
	}
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	if (zup){
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1,0,0));
	}
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0,1,0));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1,0,0));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0,0,1));
	model = glm::shearX3D(model,shear,0.0f);
	return model;
}

void Component3D::draw(GLboolean drawChilds) {
	glm::mat4 model;
	mesh->draw(getTransormMatrix(model));
	if (drawChilds){
		for(Component3D* child : childs) {
			child->draw();
		}
	}
}
