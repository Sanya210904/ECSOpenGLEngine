#pragma once

#include <glad/glad.h>

class VertexBuffer {
public:
    VertexBuffer(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
    unsigned int getID() const;

private:
    unsigned int m_rendererID;
};
