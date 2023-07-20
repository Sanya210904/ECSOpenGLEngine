#include <graphics/IndexBuffer.hpp>

#include <iostream>

IndexBuffer::IndexBuffer(const void* data, unsigned int count, GLenum usage)
    :
    m_count(count)
{
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usage);
}

IndexBuffer::~IndexBuffer()
{
    //glDeleteBuffers(1, &m_rendererID);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int IndexBuffer::getCount() const
{
    return m_count;
}

unsigned int IndexBuffer::getID() const
{
    return m_rendererID;
}
