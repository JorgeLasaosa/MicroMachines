#include "Texture.h"

#include <iostream>

// Constructor

Texture::Texture() :
    //WIDTH(0), HEIGHT(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST)
    WIDTH(0), HEIGHT(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST)
{
    glGenTextures(1, &this->ID);
}

Texture::Texture(GLuint internal_format, GLuint image_format, GLuint wrap_s, GLuint wrap_t, GLuint filter_min, GLuint filter_max)
    : WIDTH(0), HEIGHT(0), Internal_Format(internal_format), Image_Format(image_format), Wrap_S(wrap_s), Wrap_T(wrap_t),
      Filter_Min(filter_min), Filter_Max(filter_max)
{
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
