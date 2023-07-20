#pragma once

#include <glm/glm.hpp>
#include <unordered_map>

class Camera
{
public:
    enum class Action;
    enum class SpeedMode : char;

    Camera();
    Camera(const glm::mat4 projection);

    void onUpdate(float delta);

    void move(float value, float speed, float speedMulti, Action action);
    void setSpeedMode(SpeedMode speedMode);

    void setPerspective(float fov, float nearClip, float farClip);
    void setOrthographic(float size, float nearClip, float farClip);
    void setViewportSize(unsigned int width, unsigned int height);

    const glm::mat4& getProjection() const;
    const glm::mat4& getView() const;

    enum class Action
    {
        FORWARD,
        BACKWARDS,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum class SpeedMode : char
    {
        NORMAL,
        SLOW,
        FAST
    };

    enum class ProjectionType
    {
        Perspective,
        Orthographic
    };

    struct Transforms
    {
        glm::vec3 translation {0.0f, 0.0f, -1.0f};
        glm::vec3 forward {0.0f, 0.0f, 1.0f};
        glm::vec3 right {1.0f, 0.0f, 0.0f};
        glm::vec3 up {0.0f, 1.0f, 0.0f};
        glm::vec2 eulerAngles {0.0f, glm::radians(90.0f)}; // yaw, pitch
    } transforms;

private:
    struct Perspective
    {
        float FOV { glm::radians(45.0f) };
        float near { 0.01f };
        float far { 100.0f };
    } m_perspective;

    struct Orthographic
    {
        float size { 10.0f };
        float near { -1.0f };
        float far { 1.0f };
    } m_orthographic;

    struct SpeedSpecs
    {
        float extent { 0.0f };
        float speed { 2.0f };
        float speedMulti { 2.0f };
    };

public:
    Perspective getPerspective();
    Orthographic getOrthographic();

    ProjectionType getProjectionType();
    void setProjectionType(ProjectionType type);

private:
    void recalculateProjection();

    ProjectionType m_projectionType { ProjectionType::Perspective };
    glm::mat4 m_projection { 1.0f };
    glm::mat4 m_view { 1.0f };

    SpeedMode m_speedMode;
    std::unordered_map<Action, SpeedSpecs> m_moveSet;

    float m_aspectRatio { 1.0f };
};