#pragma once

#include <vector>

#include <graphics/VertexArrayBuffer.hpp>
#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>

#include <core/ResourceManager.hpp>

struct Vertex
{
    glm::vec3 position {};
    glm::vec3 normal {};
    glm::vec2 texCoords {};

    bool operator==(const Vertex& other) const;
};

struct Mesh
{
    Mesh(const std::shared_ptr<VertexArrayBuffer>& vao);
    void draw(std::shared_ptr<Shader> shader) const;
    
    std::pair<std::string, std::shared_ptr<Texture>> texture;
    std::shared_ptr<VertexArrayBuffer> vao;
};

class Model
{
public:
    Model() = default;
    Model(const std::string& filepath);

    void load(const std::string& filepath);
    void draw();

    std::shared_ptr<Shader> shader;
    
private:
    std::string m_filepath;
    std::vector<Mesh> m_meshes;
};