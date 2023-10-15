#pragma once

#include "glm/gtc/constants.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

struct CameraComponent
{
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    ~CameraComponent() = default;

    enum class ProjectionType
    {
        Perspective,
        Orthographic
    };

    glm::mat4 getProj(float aspect) const
    {
        switch (projectionType)
        {
        case ProjectionType::Perspective:
            return glm::perspective(glm::radians(fov), aspect, nearBound, farBound);

        case ProjectionType::Orthographic:
            return glm::ortho(-aspect, aspect, -1.0f, 1.0f, nearBound, farBound);
        }
    }

    static glm::mat4 getView(const glm::mat4& tMat)
    {
        return glm::inverse(tMat);
    }

    static glm::vec3 getForward(const glm::mat4& tMat)
    {
        // return glm::normalize(glm::vec3(getView(tMat)[2])) * glm::vec3(1, 1, -1);
        return glm::normalize(-glm::vec3(tMat[2]));
    }

    static glm::vec3 getRight(const glm::mat4& tMat)
    {
        // return glm::normalize(glm::vec3(getView(tMat)[0]));
        return glm::normalize(glm::vec3(tMat[0]));
    }

    static glm::vec3 getUp(const glm::mat4& tMat)
    {
        // return glm::normalize(glm::vec3(getView(tMat)[1]));
        return glm::normalize(glm::vec3(tMat[1]));
    }

    float fov { 45.0f };
    float nearBound { 0.001f };
    float farBound { 100.0f };

    ProjectionType projectionType { ProjectionType::Perspective };
};
