#pragma once

#include <glad/glad.h>
#include <vector>
#include <iostream>

struct VertexBufferElement
{
    GLenum type;
    unsigned int count;
    GLboolean normalized;

    static unsigned int getSizeOfType(GLenum type)
    {
        switch(type)
        {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_BYTE: return 1;
        }

#define GET_VARIABLE_NAME(Variable) (#Variable)
        std::cout << "ERROR::GRAPHICS::VERTEX_BUFFER_LAYOUT::UNKNOWN_TYPE::" << GET_VARIABLE_NAME(type) << "\n";
#undef GET_VARIABLE_NAME

        return 0;
    }
};


class VertexBufferLayout
{
public:
    VertexBufferLayout()
        :
        m_stride(0) { }

    template<typename T>
    void push(unsigned int count) { }

    inline const std::vector<VertexBufferElement>& getElements() const
    {
        return m_elements;
    }
    inline unsigned int getStride() const
    {
        return m_stride;
    }

private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;
};

template<>
inline void VertexBufferLayout::push<float>(unsigned int count)
{
    m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template<>
inline void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}
