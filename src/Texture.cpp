#include "Texture.h"

#include <iostream>

// Constructor

Texture::Texture() :
    WIDTH(0), HEIGHT(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT),
    Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR) {
        glGenTextures(1, &this->ID);
    }

void Texture::generate(GLuint width, GLuint height, unsigned char* data) {
    this->WIDTH = width;
    this->HEIGHT = height;
    // Create Texture
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);

    // Set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    // Unbind Texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, this->ID);
}