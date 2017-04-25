#include "Mesh3DRenderer.h"

#include <iostream>

Mesh3DRenderer::Mesh3DRenderer(Shader& shader, const char* modelFile, Camera* camera, const GLint windowWidth, const GLint windowHeight)
    : shader(shader), windowWidth(windowWidth), windowHeight(windowHeight), camera(camera)
{
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

    std::ifstream infile(modelFile);
    numVertices, numFaces;
    infile >> numVertices >> numFaces;
    GLfloat vertices [numVertices * 6];
    GLint faces [numFaces * 3];
    GLint vcount = 0;

    while(vcount < numVertices){
    	GLfloat vx, vy, vz, _dc;
    	GLint r, g, b;
    	infile >> vx >> vy >> vz >> _dc >> _dc >> _dc >> r >> g >> b;
    	vertices[vcount*6] = vx;
    	vertices[vcount*6+1] = vy;
    	vertices[vcount*6+2] = vz;
    	vertices[vcount*6+3] = (GLfloat) r/255;
    	vertices[vcount*6+4] = (GLfloat) g/255;
    	vertices[vcount*6+5] = (GLfloat) b/255;
    	vcount++;
    }
    GLint fcount = 0;
    while(fcount < numFaces){
    	GLint v1, v2, v3, _dc;
    	infile >> _dc >> v1 >> v2 >> v3;
    	faces[fcount*3] = v1;
    	faces[fcount*3+1] = v2;
    	faces[fcount*3+2] = v3;
    	fcount++;
    }
    infile.close();

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

	// // glGenBuffers(2, &VBO_tex);
	// // glBindBuffer(GL_ARRAY_BUFFER, VBO_tex);
	// // glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	// glEnableVertexAttribArray(2);   // TexCoords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);
}

void Mesh3DRenderer::draw(glm::mat4 model) {// glm::vec2 img_size

	this->shader.use();
	this->shader.setMatrix4("view", glm::mat4(1.0));
		

	if (camera->isEnabled()){
		// View Matrix
		this->shader.setMatrix4("view", this->camera->getViewMatrix());

		// Projection Matrix
		glm::mat4 projection = this->camera->getPerspective();
		this->shader.setMatrix4("projection", projection);

	} else {
		glm::mat4 projection = this->camera->getOrthogonal();
		this->shader.setMatrix4("projection", projection);
	}


	this->shader.setMatrix4("model", model);

	//this->shader.setVector4f("frame", frame.getTextureCoords());

	// Render texture quad
	// glActiveTexture(GL_TEXTURE0);
	// texture.bind();

	glBindVertexArray(this->quadVAO);
	glDrawElements(GL_TRIANGLES, numFaces*3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
