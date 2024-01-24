#pragma once

#include <entt/entt.hpp>
#include <unordered_map>
#include <utils/UUID.hpp>

class Entity;

class Scene {
public:
    Scene();
    ~Scene();

    Entity createEntity(const std::string& name);
    void destroyEntity(Entity entity);

    Entity getEntityByUUID(UUID uuid);

    template <typename... Components>
    auto getAllEntitiesWith() {
        return m_registry.view<Components...>();
    }

private:
    entt::registry m_registry;
    std::unordered_map<UUID, entt::entity> m_entityMap;

    friend class Entity;
};
