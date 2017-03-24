#ifndef SHADER_H
#define SHADER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
	// The program ID
	GLuint program;

    //Constructor
	Shader() {}

	// Compiles the shader from the given source code
	void compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource);

	// Use the program
	Shader& use();

	// Utility Functions
	void setFloat    (const GLchar *name, GLfloat value, GLboolean useShader = false);
    void setInteger  (const GLchar *name, GLint value, GLboolean useShader = false);
    void setVector2f (const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void setVector2f (const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
    void setVector3f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void setVector3f (const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
    void setVector4f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void setVector4f (const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
    void setMatrix4  (const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
};

#endif  // SHADER_H
