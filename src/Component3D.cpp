#include "Component3D.h"

#include <iostream>

Component3D::Component3D(Shader& shader, const GLint windowWidth, const GLint windowHeight, const char* modelFile)
    : position(glm::vec3(0,0,0)), rotation(glm::vec3(0,0,1)), rotationAngle(0), scale(glm::vec3(1,1,1)), parent(nullptr)
{
	this->mesh = new Mesh3DRenderer(shader, modelFile);
}

Component3D::Component3D(Shader& shader, const GLint windowWidth, const GLint windowHeight, Mesh3DRenderer* mesh)
    : position(glm::vec3(0,0,0)), rotation(glm::vec3(0,0,1)), rotationAngle(0), scale(glm::vec3(1,1,1)), parent(nullptr), mesh(mesh)
{
}

Component3D::~Component3D() {
	delete mesh;
}

void Component3D::setParent(Component3D* parent) {
	this->parent = parent;
	parent->childs.push_back(this);
}

void Component3D::setPosition(glm::vec3 position){
	this->position = position;
}

void Component3D::setRotation(glm::vec3 rotation, GLfloat rotationAngle){
	this->rotation = rotation;
	this->rotationAngle = rotationAngle;
}

void Component3D::setScale(glm::vec3 scale){
	this->scale = scale;
}


glm::mat4 Component3D::getTransormMatrix(glm::mat4 model){

	if (parent != nullptr){
		model = parent->getTransormMatrix(model);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		model = glm::rotate(model, glm::radians(rotationAngle), rotation);

	} else {

		model = glm::translate(model, position);
		model = glm::scale(model, scale);   // Last scale
		model = glm::rotate(model, glm::radians(rotationAngle), rotation);

	}
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
