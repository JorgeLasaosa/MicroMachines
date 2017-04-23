#include "SpriteRenderer.h"

#include <iostream>

SpriteRenderer::SpriteRenderer(Shader& shader, const GLint windowWidth, const GLint windowHeight)
    : shader(shader), windowWidth(windowWidth), windowHeight(windowHeight)
{
    this->squareSize = windowHeight / 18.0f;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::initRenderData() {
	// Configure VAO/VBO
	GLuint VBO;
	GLfloat vertices[] {
		// Pos            // Tex
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);   // Position

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);   // TexCoords

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::drawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, SpriteFrame frame) {// glm::vec2 img_size

	// Prepare transformations
	this->shader.use();
	glm::mat4 model;

	// First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
	model = glm::translate(model, glm::vec3(position * squareSize, 0.0f));

//	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
//	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	model = glm::scale(model, glm::vec3(size * squareSize, 1.0f));   // Last scale

	this->shader.setMatrix4("model", model);
	this->shader.setVector4f("frame", frame.getTextureCoords());

	// Render texture quad
	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::drawSprite(Texture& texture, glm::vec3 position, glm::vec2 size, SpriteFrame frame) {// glm::vec2 img_size

	// Prepare transformations
	this->shader.use();
	glm::mat4 model;

	// First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
	model = glm::translate(model, position * squareSize);

//	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
//	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	model = glm::scale(model, glm::vec3(size * squareSize, 1.0f));   // Last scale

	this->shader.setMatrix4("model", model);
	this->shader.setVector4f("frame", frame.getTextureCoords());

	// Render texture quad
	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
