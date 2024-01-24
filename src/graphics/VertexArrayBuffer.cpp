#include <graphics/IndexBuffer.hpp>
#include <graphics/VertexArrayBuffer.hpp>
#include <graphics/VertexBuffer.hpp>
#include <graphics/VertexBufferLayout.hpp>

VertexArrayBuffer::VertexArrayBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    glCreateVertexArrays(1, &m_rendererID);

    glBindVertexArray(m_rendererID);
    vb.bind();

    const auto& elements = layout.getElements();
    const unsigned int stride = layout.getStride();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, stride,
                              (const void*)(offset));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }

    vb.unbind();
    glBindVertexArray(0);
}

VertexArrayBuffer::~VertexArrayBuffer() {
    // glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArrayBuffer::bind() const { glBindVertexArray(m_rendererID); }

void VertexArrayBuffer::unbind() const { glBindVertexArray(0); }

void VertexArrayBuffer::setIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) {
    bind();
    indexBuffer->bind();
    m_indexBuffer = indexBuffer;
}

void VertexArrayBuffer::bindIndex() const { m_indexBuffer->bind(); }

void VertexArrayBuffer::unbindIndex() const { m_indexBuffer->unbind(); }

std::shared_ptr<IndexBuffer>& VertexArrayBuffer::getIndexBuffer() { return m_indexBuffer; }

unsigned int VertexArrayBuffer::getID() const { return m_rendererID; }
