#pragma once

#include "glm/gtc/quaternion.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

struct TransformComponent {
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& translation)
        : translation(translation) {}

    glm::mat4 getTransform() const {
        glm::mat4 rot = glm::toMat4(orientation);
        return glm::translate(glm::mat4(1.0f), translation) * rot *
               glm::scale(glm::mat4(1.0f), scale);
    }

    /*
     * Accepts an euler angles rotation as vector where:
     * x - pitch,
     * y - yaw,
     * z - roll
     */
    glm::quat eulerRotate(const glm::vec3& rotation) { return orientation = glm::quat(rotation); }

    /*
     * Get euler angles rotation as vector where:
     * x - pitch,
     * y - yaw,
     * z - roll
     */
    glm::vec3 getEulerRotation() { return glm::eulerAngles(orientation); }

    glm::vec3 translation{0.0f, 0.0f, 0.0f};
    glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
};
