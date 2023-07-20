#pragma once

#include <memory>
#include <string>

class Texture;
class ResourceManager
{
public:
    static std::shared_ptr<Texture>& loadTexture(const std::string& path, bool gamma = true);
    static std::shared_ptr<Texture>& getTexture(const std::string& path, bool gamma = true);
};