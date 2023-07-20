#pragma once

#include <glad/glad.h>
#include <memory>

class VertexBuffer;
class VertexBufferLayout;
class IndexBuffer;

class VertexArrayBuffer
{
public:
    VertexArrayBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    ~VertexArrayBuffer();

    void bind() const;
    void unbind() const;
    void setIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer);
    void bindIndex() const;
    void unbindIndex() const;
    std::shared_ptr<IndexBuffer>& getIndexBuffer();
    unsigned int getID() const;

private:
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    unsigned int m_rendererID;
};