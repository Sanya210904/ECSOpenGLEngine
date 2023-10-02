#include <graphics/Mesh.hpp>

#include <filesystem>
#include <functional>
#include <unordered_map>
#include <glm/gtx/hash.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <graphics/VertexBuffer.hpp>
#include <graphics/IndexBuffer.hpp>
#include <graphics/VertexBufferLayout.hpp>

namespace std
{
    template<>
    struct hash<Vertex>
    {
        size_t operator()(const Vertex& vertex) const noexcept
        {
            const size_t h1 = std::hash<glm::vec3>{}(vertex.position);
            const size_t h2 = std::hash<glm::vec3>{}(vertex.normal);
            const size_t h3 = std::hash<glm::vec2>{}(vertex.texCoords);

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

Submesh::Submesh(const std::shared_ptr<VertexArrayBuffer>& vao)
    :
    vao(vao) { }

void Submesh::draw(std::shared_ptr<Shader> shader) const
{
    //unsigned int diffuseNr = 1;
    //unsigned int specularNr = 1;

    // for (unsigned int i = 0; i < textures.size(); ++i)
    // {
    //     textures[i].second->bind(i);

    //     std::string name = textures[i].first;
    //     std::string number = (name == "textureDiffuse") 
    //         ? std::to_string(diffuseNr++)
    //         : std::to_string(specularNr++);
    //     shader->setUniform("material." + name + number, i);
    // }

    if (texture.second.get())
    {
        texture.second->bind();
        shader->setUniform("textureDiffuse", 0);
        glActiveTexture(GL_TEXTURE0);
    }

    vao->bind();
    vao->bindIndex();
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(vao->getIndexBuffer()->getCount()), GL_UNSIGNED_INT, 0);
    vao->unbindIndex();
    vao->unbind();
}

Mesh::Mesh(const std::string& filepath)
{
    shader = std::make_shared<Shader>("res/shaders/model.vert", "res/shaders/model.frag");

    load(filepath);
}

void Mesh::draw()
{
    for (const auto& mesh : m_submeshes)
        mesh.draw(shader);
}

void Mesh::load(const std::string& filepath)
{
    std::filesystem::path path = filepath;

    if (!std::filesystem::exists(path))
    {
        std::cout << "ERROR::GRAPHICS::MODEL::PATH_DO_NOT_EXIST";
        return;
    }

    if (path.extension() != ".obj")
    {
        std::cout << "ERROR::GRAPHICS::MODEL::WRONG_FILE_FORMAT::only .obj is accepted";
        return;
    }

    tinyobj::ObjReaderConfig readerConfig;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filepath, readerConfig))
    {
        if (!reader.Error().empty())
            std::cout << "ERROR::GRAPHICS::MODEL::PARSE_ERROR::" << reader.Error();
        return;
    }

    if (!reader.Warning().empty())
        std::cout << "WARNING::GRAPHICS::MODEL::" << reader.Warning();

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    for (const auto& shape : shapes)
    {
        std::vector<Vertex> vertices {};
        std::vector<unsigned int> indices {};
        std::unordered_map<Vertex, unsigned int> uniqueVertices {};

        int materialId = -1;
        size_t indexOffset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
        {
            size_t fv = static_cast<size_t>(shape.mesh.num_face_vertices[f]);

            for (size_t v = 0; v < fv; ++v)
            {
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];
                
                Vertex vertex;

                vertex.position.x = attrib.vertices[3 * idx.vertex_index + 0];
                vertex.position.y = attrib.vertices[3 * idx.vertex_index + 1];
                vertex.position.z = attrib.vertices[3 * idx.vertex_index + 2];

                if (idx.normal_index >= 0)
                {
                    vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
                    vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
                    vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];
                }

                if (idx.texcoord_index >= 0)
                {
                    vertex.texCoords.x = attrib.texcoords[2 * idx.texcoord_index + 0];
                    vertex.texCoords.y = attrib.texcoords[2 * idx.texcoord_index + 1];
                }

                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<unsigned int>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }

            indexOffset += fv;

            materialId = shape.mesh.material_ids[f];
        }

        VertexBuffer vbo(vertices.data(), static_cast<unsigned int>(vertices.size()) * sizeof(Vertex));
        VertexBufferLayout vbl;
        vbl.push<float>(3);
        vbl.push<float>(3);
        vbl.push<float>(2);

        auto vao = std::make_shared<VertexArrayBuffer>(vbo, vbl);

        auto ebo = std::make_shared<IndexBuffer>(indices.data(), static_cast<unsigned int>(indices.size()));

        vao->setIndexBuffer(ebo);

        auto& submesh = m_submeshes.emplace_back(vao);

        if (materialId >= 0)
        {
            const auto& material = materials.at(materialId);

            if (!material.diffuse_texname.empty())
            {
                submesh.texture = std::make_pair(std::string("textureDiffuse"), ResourceManager::getTexture((path.parent_path() / material.diffuse_texname).string()));
            }
        }
    }
}

bool Vertex::operator==(const Vertex &other) const
{
    return position == other.position &&
        normal == other.normal &&
        texCoords == other.texCoords;
}
