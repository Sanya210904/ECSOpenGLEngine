#include "CameraFactory.hpp"

CameraComponent CameraFactory::createPerspectiveCamera(unsigned int scrWidth, unsigned int scrHeight, float fov, float nearClip, float farClip)
{
    Camera camera;
    camera.setViewportSize(scrWidth, scrHeight);
    camera.setPerspective(fov, nearClip, farClip);

    return CameraComponent(camera);
}

CameraComponent CameraFactory::createOrthographicCamera(unsigned int scrWidth, unsigned int scrHeight, float size, float nearClip, float farClip)
{
    return CameraComponent();
}
