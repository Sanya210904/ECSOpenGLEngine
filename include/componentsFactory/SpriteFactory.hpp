#pragma once

#include <components/SpriteComponent.hpp>
#include <entt/entt.hpp>

class SpriteFactory {
public:
    static SpriteComponent createSprite(const std::string& path, bool keepAspect = true);
    static SpriteComponent createSprite(std::shared_ptr<Texture> texture, bool keepAspect = true);
    static SpriteComponent createSprite(std::shared_ptr<Texture> texture,
                                        std::shared_ptr<Shader> shader, bool keepAspect = true);
};
