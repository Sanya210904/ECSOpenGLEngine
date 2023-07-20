#pragma once

#include "Scene.hpp"
#include <components/TagComponent.hpp>

class Entity
{
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene);
    Entity(const Entity& other) = default;

    template<typename T, typename... Args>
    T& addComponent(Args&&... args)
    {
        return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
    }

    template<typename T>
    T& getComponent()
    {
        return m_scene->m_registry.get<T>(m_entityHandle);
    }

    template<typename T>
    void removeComponent()
    {
        m_scene->m_registry.remove<T>(m_entityHandle);
    }

    template<typename T>
    bool hasComponent()
    {
        return m_scene->m_registry.all_of<T>(m_entityHandle);
    }

    const std::string& getName() { return getComponent<TagComponent>().tag; }

    operator bool() const { return m_entityHandle != entt::null; }
    operator entt::entity() const { return m_entityHandle; }

private:
    entt::entity m_entityHandle { 0 };
    Scene* m_scene = nullptr;
};