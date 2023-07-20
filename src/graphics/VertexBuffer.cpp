#include <graphics/VertexBuffer.hpp>

VertexBuffer::VertexBuffer(const void* data, unsigned int size, GLenum usage)
{
    glGenBuffers(1, &m_rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer()
{
	//glDeleteBuffers(1, &m_rendererID);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int VertexBuffer::getID() const
{
    return m_rendererID;
}
