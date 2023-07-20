#pragma once

#include <entt/entt.hpp>
#include <components/CameraComponent.hpp>

class CameraFactory
{
public:
    static CameraComponent createPerspectiveCamera(unsigned int scrWidth, unsigned int scrHeight, float fov, float nearClip, float farClip);
    static CameraComponent createOrthographicCamera(unsigned int scrWidth, unsigned int scrHeight, float size, float nearClip, float farClip);
};