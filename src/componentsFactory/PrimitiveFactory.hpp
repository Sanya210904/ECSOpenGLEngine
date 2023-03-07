#pragma once

#include <entt/entt.hpp>
#include <components/PrimitiveComponent.hpp>

class PrimitiveFactory
{
public:
    static PrimitiveComponent createRect(glm::vec4 color);
    static PrimitiveComponent createPolygon(glm::vec4 color, unsigned int vertexCount = 4);
};