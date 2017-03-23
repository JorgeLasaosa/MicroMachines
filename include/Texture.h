#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture {
    public:
        // Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
        GLuint ID;

        // Texture image dimensions
        GLuint WIDTH, HEIGHT; // Width and height of loaded image in pixels

        // Texture Format
        GLuint Internal_Format; // Format of texture object
        GLuint Image_Format; // Format of loaded image

        // Texture configuration
        GLuint Wrap_S; // Wrapping mode on S axis
        GLuint Wrap_T; // Wrapping mode on T axis
        GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
        GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels

        // Constructor (sets default texture modes)
        Texture();

        // Constructor (given texture modes)
        Texture(GLuint internal_format, GLuint image_format, GLuint wrap_s, GLuint wrap_t, GLuint filter_min, GLuint filter_max);

        // Generates texture from image data
        void generate(GLuint width, GLuint height, unsigned char* data);

        // Binds the texture as the current active GL_TEXTURE_2D texture object
        void bind() const;
};

#endif // TEXTURE_H
