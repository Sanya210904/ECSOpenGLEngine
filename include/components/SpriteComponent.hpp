#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>
#include <graphics/VertexArrayBuffer.hpp>

#include <memory>

struct SpriteComponent {
    SpriteComponent() = default;
    SpriteComponent(const SpriteComponent&) = default;
    SpriteComponent(GLenum type, std::shared_ptr<VertexArrayBuffer> vao,
                    std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
        : type(type)
        , vao(vao)
        , shader(shader)
        , texture(texture) {}

    GLenum type;
    unsigned int vertexCount{4};
    std::shared_ptr<Texture> texture;
    std::shared_ptr<VertexArrayBuffer> vao;
    std::shared_ptr<Shader> shader;
};
