#ifndef RENDERER_H
#define RENDERER_H


class Renderer
{
    public:
        Renderer(Shader& shader, const GLint windowWidth, const GLint windowHeight);
        virtual ~Renderer();

    private:
};

#endif // RENDERER_H
