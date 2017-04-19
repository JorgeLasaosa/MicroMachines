#include "Component3D.h"

#include <iostream>

Component3D::Component3D(Shader& shader, const GLint windowWidth, const GLint windowHeight, const char* modelFile)
    : position(glm::vec3(0,0,0)), rotation(glm::vec3(0,0,0)), rotationAngle(0), scale(glm::vec3(0,0,0)) 
{
	this->mesh = new Mesh3DRenderer(shader,windowWidth, windowHeight, modelFile);
}

Component3D::~Component3D() {
	delete mesh;
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

void Component3D::draw() {
	mesh->draw(position, rotation, rotationAngle, scale);
}
