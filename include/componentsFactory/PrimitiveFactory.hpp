#pragma once

#include <components/PrimitiveComponent.hpp>
#include <entt/entt.hpp>

class PrimitiveFactory {
public:
    static PrimitiveComponent createRect(glm::vec4 color);
    static PrimitiveComponent createPolygon(glm::vec4 color, unsigned int vertexCount = 4);
};
