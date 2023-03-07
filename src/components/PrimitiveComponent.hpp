#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <graphics/VertexArrayBuffer.hpp>
#include <graphics/Shader.hpp>

#include <memory>

struct PrimitiveComponent
{
    PrimitiveComponent() = default;
    PrimitiveComponent(const PrimitiveComponent&) = default;
    PrimitiveComponent(glm::vec4 color, GLenum type, unsigned int vertexCount, std::shared_ptr<VertexArrayBuffer> vao, std::shared_ptr<Shader> shader)
        :
        color(color),
        type(type),
        vertexCount(vertexCount),
        vao(vao),
        shader(shader)
    { }

    glm::vec4 color;
    GLenum type;
    unsigned int vertexCount;
    std::shared_ptr<VertexArrayBuffer> vao;
    std::shared_ptr<Shader> shader;
};