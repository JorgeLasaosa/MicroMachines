#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer() {
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::initRenderData() {

}

SpriteRenderer::SpriteRenderer(Shader& shader) {

}
