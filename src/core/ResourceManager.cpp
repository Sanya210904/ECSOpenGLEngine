#include "ResourceManager.hpp"

#include <unordered_map>

#include <graphics/Texture.hpp>
#include <utils/StringUtils.hpp>

inline static std::unordered_map<std::string, std::shared_ptr<Texture>, StringUtils::StringHash, std::equal_to<>> m_textureMap;

std::shared_ptr<Texture>& ResourceManager::loadTexture(const std::string& path, bool gamma)
{
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(path, gamma);

    m_textureMap.emplace(path, texture);
    return m_textureMap[path];
}

std::shared_ptr<Texture>& ResourceManager::getTexture(const std::string& path, bool gamma)
{
    const auto& it = m_textureMap.find(path);
    if (it != m_textureMap.end() && it->second->isGammaCorrected() == gamma) return it->second;

    return loadTexture(path, gamma);
}
