#pragma once

#include <string>
#include <glad/glad.h>

class TextureArray
{
public:
    TextureArray(const std::string& filePath, int tileWidth, int tileHeight);
    ~TextureArray();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    unsigned int getID() const;
    int getWidth() const;
    int getHeight() const;

private:
    unsigned int m_textureID;
    int m_width,
        m_height;
};