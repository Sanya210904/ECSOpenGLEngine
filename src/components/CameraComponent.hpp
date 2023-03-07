#pragma once

#include <scene/Camera.hpp>

struct CameraComponent
{
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    CameraComponent(const Camera& camera)
        :
        camera(camera) { }

    Camera camera;
    bool fixedAspectRatio = false;
};