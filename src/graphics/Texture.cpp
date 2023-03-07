#include "Texture.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const std::string &filePath, bool gamma)
    :
    m_gamma(gamma)
{
    stbi_set_flip_vertically_on_load(true);

    int nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &m_width, &m_height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "ERROR::GRAPHICS::TEXTURE::OPEN_FAILED::PATH_" << filePath << std::endl;
        return;
    }

    auto [dataFormat, internalFormat] = [nrChannels, gamma]()
    {
        struct _{ GLenum dataFormat, internalFormat; };
        switch(nrChannels)
        {
        case 1: return _{GL_RED, GL_RED};
        case 2: return _{GL_RG, GL_RG};
        case 3: return _{GL_RGB, gamma ? (GLenum)GL_SRGB : (GLenum)GL_RGB};
        case 4: return _{GL_RGBA, gamma ? (GLenum)GL_SRGB_ALPHA : (GLenum)GL_RGBA};
        default: return _{0, 0};
        }
    }();

    glGenTextures(1, &m_textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_textureID);
}

void Texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::getID() const
{
    return m_textureID;
}

bool Texture::isGammaCorrected() const
{
    return m_gamma;
}

int Texture::getWidth() const
{
    return m_width;
}

int Texture::getHeight() const
{
    return m_height;
}
