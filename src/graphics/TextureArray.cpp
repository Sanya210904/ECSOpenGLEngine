#include <graphics/TextureArray.hpp>

#include <iostream>

// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureArray::TextureArray(const std::string& filePath, int tileWidth, int tileHeight) {
    stbi_set_flip_vertically_on_load(false);

    int nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &m_width, &m_height, &nrChannels, 0);
    if (!data) {
        std::cout << "ERROR::GRAPHICS::TEXTURE_ARRAY::OPEN_FAILED::PATH_" << filePath << std::endl;
        return;
    }

    int tilesCountX = m_width / tileWidth;
    int tilesCountY = m_height / tileHeight;
    int tilesCount = tilesCountX * tilesCountY;

    glGenTextures(1, &m_textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA, tileWidth, tileHeight, tilesCount);

    unsigned int tempTextureID = 0;
    glGenTextures(1, &tempTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tempTextureID);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, m_width, m_height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    for (int i = 0; i < tilesCount; ++i) {
        int x = (i % tilesCountX) * tileWidth;
        int y = (i / tilesCountX) * tileHeight;
        glCopyImageSubData(tempTextureID, GL_TEXTURE_2D, 0, x, y, 0, m_textureID,
                           GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, tileWidth, tileHeight, 1);
    }

    glDeleteTextures(1, &tempTextureID);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    stbi_image_free(data);
}

TextureArray::~TextureArray() { glDeleteTextures(1, &m_textureID); }

void TextureArray::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextureArray::unbind() const { glBindTexture(GL_TEXTURE_2D_ARRAY, 0); }

unsigned int TextureArray::getID() const { return m_textureID; }

int TextureArray::getWidth() const { return m_width; }

int TextureArray::getHeight() const { return m_height; }
