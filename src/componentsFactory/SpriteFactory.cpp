#include <componentsFactory/SpriteFactory.hpp>

#include <array>

#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>
#include <graphics/VertexBuffer.hpp>
#include <graphics/VertexBufferLayout.hpp>
#include <graphics/VertexArrayBuffer.hpp>

#include <core/ResourceManager.hpp>

SpriteComponent SpriteFactory::createSprite(const std::string& path, bool keepAspect)
{
    auto texture = ResourceManager::getTexture(path, false);

    return createSprite(texture, keepAspect);
}

SpriteComponent SpriteFactory::createSprite(std::shared_ptr<Texture> texture, bool keepAspect)
{
    std::array<float, 20> vertices;
    float aspect = static_cast<float>(texture->getWidth()) / static_cast<float>(texture->getHeight());
    if (keepAspect)
        vertices =
        {
            -0.5f * aspect, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f * aspect, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f * aspect, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f * aspect, -0.5f, 0.0f, 1.0f, 0.0f
        };
    else
        vertices =
        {
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f
        };

    VertexBuffer vbo(vertices.data(), static_cast<unsigned int>(vertices.size()) * sizeof(float), GL_STATIC_DRAW);
    VertexBufferLayout  vbl;
    vbl.push<float>(3);
    vbl.push<float>(2);

    std::shared_ptr vao = std::make_shared<VertexArrayBuffer>(vbo, vbl);

    std::shared_ptr shader = std::make_shared<Shader>("res/shaders/texture.vert", "res/shaders/texture.frag");
    vao->unbind();
    vbo.unbind();
    
    return SpriteComponent(GL_TRIANGLE_STRIP, vao, shader, texture);
}
