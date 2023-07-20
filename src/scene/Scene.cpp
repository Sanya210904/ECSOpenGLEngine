#include <scene/Scene.hpp>

#include <scene/Entity.hpp>
#include <components/IDComponent.hpp>
#include <components/TagComponent.hpp>
#include <components/TransformComponent.hpp>

Scene::Scene()
{
}

Scene::~Scene()
{
}

Entity Scene::createEntity(const std::string& name)
{
    UUID uuid;
    Entity entity = { m_registry.create(), this };
    entity.addComponent<IDComponent>(uuid);
    entity.addComponent<TransformComponent>();
    auto& tag = entity.addComponent<TagComponent>();
    tag.tag = name.empty() ? "Entity" : name;
    
    m_entityMap[uuid] = entity;

    return entity;
}

void Scene::destroyEntity(Entity entity)
{
    m_registry.destroy(entity);
}

Entity Scene::getEntityByUUID(UUID uuid)
{
    if (m_entityMap.find(uuid) != m_entityMap.end())
        return { m_entityMap.at(uuid), this };

    return { };
}
