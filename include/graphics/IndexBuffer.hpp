#pragma once

#include <glad/glad.h>
#include <vector>

class IndexBuffer {
public:
    IndexBuffer(const void* data, unsigned int count, GLenum usage = GL_STATIC_DRAW);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    unsigned int getCount() const;
    unsigned int getID() const;

private:
    unsigned int m_rendererID;
    unsigned int m_count;
};
