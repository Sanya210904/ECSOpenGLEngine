#pragma once

#include <string>
#include <glad/glad.h>

class Texture
{
public: 
    Texture(const std::string& filePath, bool gamma = true);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    unsigned int getID() const;
    bool isGammaCorrected() const;
    int getWidth() const;
    int getHeight() const;

private:
    unsigned int m_textureID;
    bool m_gamma;
    int m_width,
        m_height;
};