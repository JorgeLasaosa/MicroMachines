#include "Mesh3DRenderer.h"

#include <iostream>

Mesh3DRenderer::Mesh3DRenderer(Shader& shader, const GLint windowWidth, const GLint windowHeight, const char* modelFile)
    : shader(shader)
{
    this->squareSize = windowHeight / 18.0f;
	this->initRenderData(modelFile);
}

Mesh3DRenderer::~Mesh3DRenderer() {
	glDeleteVertexArrays(1, &this->quadVAO);
	glDeleteBuffers(2,&VBO_tex);
}

void Mesh3DRenderer::initRenderData(const char* modelFile) {
	// Configure VAO/VBO
	GLuint VBO;
	GLuint EBO;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( modelFile, 
	    aiProcess_CalcTangentSpace       | 
	    aiProcess_Triangulate            |
	    aiProcess_JoinIdenticalVertices  |
	    aiProcess_SortByPType);

	aiMesh* pengoMesh = scene->mMeshes[0];
	numVertices = pengoMesh->mNumVertices;
	numFaces = pengoMesh->mNumVertices;
	GLfloat vertices [numVertices * 6];
	GLfloat texCoords [numVertices * 2];
	GLint faces [numVertices * 3];
	for(int i = 0; i < numFaces; i++) {
		vertices[i*6] = pengoMesh->mVertices[i].x;
		vertices[i*6 + 1] = pengoMesh->mVertices[i].y;
		vertices[i*6 + 2] = pengoMesh->mVertices[i].z;
		vertices[i*6 + 3] = 1.0f;
		vertices[i*6 + 4] = 1.0f;
		vertices[i*6 + 5] = 1.0f;
		for(int j = 0; j < AI_MAX_NUMBER_OF_COLOR_SETS; j++) {
			if (pengoMesh->HasVertexColors(j)){
				vertices[i*6 + 3] = pengoMesh->mColors[j][i].r;
				vertices[i*6 + 4] = pengoMesh->mColors[j][i].g;
				vertices[i*6 + 5] = pengoMesh->mColors[j][i].b;
			}
		}
	}

	for(int i = 0; i < numFaces; i++) {
		if(pengoMesh->mFaces[i].mNumIndices == 3) {
			faces[i*3] = pengoMesh->mFaces[i].mIndices[0];
			faces[i*3 + 1] = pengoMesh->mFaces[i].mIndices[1];
			faces[i*3 + 2] = pengoMesh->mFaces[i].mIndices[2];
		}
	}

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

	glGenBuffers(2, &VBO_tex);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(2);   // TexCoords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);
}

void Mesh3DRenderer::draw(glm::vec3 position, glm::vec3 rotation, GLfloat rotationAngle, glm::vec3 scale) {// glm::vec2 img_size

	// Prepare transformations
	this->shader.use();
	glm::mat4 model;

	// First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)
	model = glm::translate(model, position * squareSize);

//	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // Move origin of rotation to center of quad
//	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

	model = glm::scale(model, scale * squareSize);   // Last scale
	//model = glm::scale(model, glm::vec3((scale * squareSize).x,(scale * squareSize).y, 1.0f));   // Last scale
	model = glm::rotate(model, glm::radians(rotationAngle), rotation);
	//model = glm::scale(model, scale * squareSize);   // TODO BEFORE ROTATION !
	

	this->shader.setMatrix4("model", model);
	//this->shader.setVector4f("frame", frame.getTextureCoords());

	// Render texture quad
	// glActiveTexture(GL_TEXTURE0);
	// texture.bind();

	glBindVertexArray(this->quadVAO);
	glDrawElements(GL_TRIANGLES, numFaces*3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
