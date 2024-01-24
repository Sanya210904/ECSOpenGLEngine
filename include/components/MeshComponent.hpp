#pragma once

#include <graphics/Mesh.hpp>

struct MeshComponent {
    MeshComponent() = default;
    MeshComponent(const MeshComponent&) = default;
    MeshComponent(std::shared_ptr<Mesh> meshGeometry)
        : meshGeometry(meshGeometry) {}

    std::shared_ptr<Mesh> meshGeometry;
};
