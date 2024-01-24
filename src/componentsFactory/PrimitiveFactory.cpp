#include <componentsFactory/PrimitiveFactory.hpp>

#include <array>
#include <vector>

#include <graphics/Shader.hpp>
#include <graphics/VertexArrayBuffer.hpp>
#include <graphics/VertexBuffer.hpp>
#include <graphics/VertexBufferLayout.hpp>

PrimitiveComponent PrimitiveFactory::createRect(glm::vec4 color) {
    std::array<float, 12> vertices{-0.5f, 0.5f,  0.0f, 0.5f, 0.5f,  0.0f,
                                   -0.5f, -0.5f, 0.0,  0.5f, -0.5f, 0.0f};

    VertexBuffer vbo(vertices.data(), static_cast<unsigned int>(vertices.size()) * sizeof(float),
                     GL_STATIC_DRAW);
    VertexBufferLayout vbl;
    vbl.push<float>(3);

    std::shared_ptr vao = std::make_shared<VertexArrayBuffer>(vbo, vbl);

    std::shared_ptr shader =
        std::make_shared<Shader>("res/shaders/default.vert", "res/shaders/default.frag");
    shader->bind();
    shader->setUniform("u_color", color);

    shader->unbind();
    vao->unbind();
    vbo.unbind();

    return PrimitiveComponent(color, GL_TRIANGLE_STRIP, 4, vao, shader);
}

PrimitiveComponent PrimitiveFactory::createPolygon(glm::vec4 color, unsigned int vertexCount) {
    std::vector<float> vertices;

    float angle = glm::two_pi<float>() / vertexCount;
    float alt = 1.0f;
    glm::vec2 tempPoint{-0.5f * sin(angle / 2.0f), -0.5f * cos(angle / 2.0f)};
    auto f = [](float rot, glm::vec2 point) {
        return glm::vec2(point.x * cos(rot) - point.y * sin(rot),
                         point.x * sin(rot) + point.y * cos(rot));
    };

    for (int i = (vertexCount - 1); i >= 0; --i) {
        vertices.insert(vertices.end(), {tempPoint.x, tempPoint.y, 0.0f});
        tempPoint = f(i * angle * alt, tempPoint);
        alt = -alt;
    }

    VertexBuffer vbo(vertices.data(), static_cast<unsigned int>(vertices.size()) * sizeof(float),
                     GL_STATIC_DRAW);
    VertexBufferLayout vbl;
    vbl.push<float>(3);

    std::shared_ptr vao = std::make_shared<VertexArrayBuffer>(vbo, vbl);

    std::shared_ptr shader =
        std::make_shared<Shader>("res/shaders/default.vert", "res/shaders/default.frag");
    shader->bind();
    shader->setUniform("u_color", color);

    shader->unbind();
    vao->unbind();
    vbo.unbind();

    return PrimitiveComponent(color, GL_TRIANGLE_STRIP, vertexCount, vao, shader);
}
