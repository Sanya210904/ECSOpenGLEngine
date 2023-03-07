#pragma once

#include <entt/entt.hpp>
#include <components/SpriteComponent.hpp>

class SpriteFactory
{
public:
    static SpriteComponent createSprite(const std::string& path, bool keepAspect = true);
    static SpriteComponent createSprite(std::shared_ptr<Texture> texture, bool keepAspect = true);
};