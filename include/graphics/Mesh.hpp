#pragma once

#include <vector>

#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>
#include <graphics/VertexArrayBuffer.hpp>

#include <core/ResourceManager.hpp>

struct Vertex {
    glm::vec3 position{};
    glm::vec3 normal{};
    glm::vec2 texCoords{};

    bool operator==(const Vertex& other) const;
};

struct Submesh {
    Submesh(const std::shared_ptr<VertexArrayBuffer>& vao);

    // To be removed by defining system
    void draw(std::shared_ptr<Shader> shader) const;

    std::pair<std::string, std::shared_ptr<Texture>> texture;
    std::shared_ptr<VertexArrayBuffer> vao;
};

class Mesh {
public:
    Mesh() = default;
    Mesh(const std::string& filepath);

    int load(const std::string& filepath);

    // To be removed by defining system
    void draw();

    std::shared_ptr<Shader> shader;

private:
    std::string m_filepath;
    std::vector<Submesh> m_submeshes;
};
