#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

struct TransformComponent
{
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& translation)
        :
        translation(translation) { }
    
    glm::mat4 getTransform() const
    {
        glm::mat4 rot = glm::toMat4(glm::quat(rotation));
        return glm::translate(glm::mat4(1.0f), translation)
                * rot
                * glm::scale(glm::mat4(1.0f), scale);
    }

    glm::vec3 translation {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation {0.0f, 0.0f, 0.0f};
    glm::vec3 scale {1.0f, 1.0f, 1.0f};
};