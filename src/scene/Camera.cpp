#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera()
{
    recalculateProjection();
}

Camera::Camera(const glm::mat4 projection)
    :
    m_projection(projection) { }

void Camera::onUpdate(float delta)
{
    for (auto& [action, val] : m_moveSet)
    {
        if (val.extent == 0.0f) continue;

        float speed;

        switch (m_speedMode)
        {
        case SpeedMode::SLOW:
            speed = val.speed / val.speedMulti;
            break;
        case SpeedMode::FAST:
            speed = val.speed * val.speedMulti;
            break;
        default:
            speed = val.speed;
            break;
        }

        switch (action)
        {
        case Action::FORWARD:
            transforms.translation += transforms.forward * delta * val.extent * speed;
            break;
        case Action::BACKWARDS:
            transforms.translation -= transforms.forward * delta * val.extent * speed;
            break;
        case Action::UP:
            transforms.translation += glm::vec3(0.0f, 1.0f, 0.0f) * delta * val.extent * speed;
            break;
        case Action::DOWN:
            transforms.translation -= glm::vec3(0.0f, 1.0f, 0.0f) * delta * val.extent * speed;
            break;
        case Action::RIGHT:
            transforms.translation += transforms.right * delta * val.extent * val.speed * speed;
            break;
        case Action::LEFT:
            transforms.translation -= transforms.right * delta * val.extent * val.speed * speed;
            break;
        default:
            break;
        }
    }

    transforms.forward = {
        glm::cos(transforms.eulerAngles.x) * glm::cos(transforms.eulerAngles.y),
        glm::sin(transforms.eulerAngles.y),
        glm::sin(transforms.eulerAngles.x) * glm::cos(transforms.eulerAngles.y)
    };

    transforms.forward = glm::normalize(transforms.forward);
    transforms.right = glm::normalize(glm::cross(transforms.forward, {0.0f, 1.0f, 0.0f}));
    transforms.up = glm::normalize(glm::cross(transforms.right, transforms.forward));

    m_view = glm::lookAt(transforms.translation, transforms.translation + transforms.forward, transforms.up);
}

void Camera::move(float value, float speed, float speedMulti, Action action)
{
    m_moveSet[action] = {
        value,
        speed,
        speedMulti
    };
}

void Camera::setSpeedMode(SpeedMode speedMode)
{
    m_speedMode = speedMode;
}

void Camera::setPerspective(float fov, float nearClip, float farClip)
{
    m_projectionType = ProjectionType::Perspective;

    m_perspective.FOV = fov;
    m_perspective.near = nearClip;
    m_perspective.far = farClip;

    recalculateProjection();
}

void Camera::setOrthographic(float size, float nearClip, float farClip)
{
    m_projectionType = ProjectionType::Orthographic;

    m_orthographic.size = size;
    m_orthographic.near = nearClip;
    m_orthographic.far = farClip;

    recalculateProjection();
}

void Camera::setViewportSize(unsigned int width, unsigned int height)
{
    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    recalculateProjection();
}

const glm::mat4 &Camera::getProjection() const
{
    return m_projection;
}

const glm::mat4 &Camera::getView() const
{
    return m_view;
}

Camera::Perspective Camera::getPerspective()
{
    return m_perspective;
}

Camera::Orthographic Camera::getOrthographic()
{
    return m_orthographic;
}

Camera::ProjectionType Camera::getProjectionType()
{
    return m_projectionType;
}

void Camera::setProjectionType(ProjectionType type)
{
    m_projectionType = type;
    recalculateProjection();
}

void Camera::recalculateProjection()
{
    if (m_projectionType == ProjectionType::Perspective)
    {
        m_projection = glm::perspective(m_perspective.FOV, m_aspectRatio, m_perspective.near, m_perspective.far);
    }
    else
    {
        float orthoTop = m_orthographic.size * 0.5f;
        float orthoBottom = -m_orthographic.size * 0.5f;
        float orthoRight = m_orthographic.size * m_aspectRatio * 0.5f;
        float orthoLeft = -m_orthographic.size * m_aspectRatio * 0.5f;

        m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographic.near, m_orthographic.far);
    }
}