#pragma once

#include <glad/glad.h>
#include <string>

class Texture {
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
    unsigned int m_textureID = 0;
    bool m_gamma;
    int m_width, m_height;
};
