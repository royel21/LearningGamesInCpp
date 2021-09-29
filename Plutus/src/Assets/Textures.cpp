#include "Textures.h"
#include "stb_image.h"
#include <iostream>
#include <filesystem>
#include <cstdio>
#include <algorithm>

namespace Plutus
{
    Texture::Texture(const std::string& id, int c, int w, int h, GLTexture tex, const std::string& _path) : name(id), path(_path)
    {
        path = _path;
        tileWidth = w;
        tileHeight = h;
        columns = c;
        totalTiles = 0;
        texWidth = tex.width;
        texHeight = tex.height;
        texId = tex.id;

        if (h)
        {
            totalTiles = static_cast<int>(floor(tex.width / w) * floor(tex.height / h));
        }
        if (c)
        {
            calculateUV();
        }
        else
        {
            mTexCount = 1;
            uvs.push_back({ 0, 1, 1, 0 });
        }
    }

    void Texture::calculateUV()
    {
        if (tileWidth > 0)
        {
            mTexCount = columns * int(texHeight / tileHeight);
            for (int i = 0; i < mTexCount; i++)
            {
                int y = i / columns;
                int x = i % columns;
                glm::vec4 UV;
                UV.x = ((float)(x * tileWidth) / (float)texWidth);
                UV.y = ((float)(y * tileHeight) / (float)texHeight);
                UV.z = ((float)(x * tileWidth + tileWidth) / (float)texWidth);
                UV.w = ((float)(y * tileHeight + tileHeight) / (float)texHeight);
                uvs.push_back(UV);
            }
        }
    }

    glm::vec4 Texture::getUV(float column, float row, float w, float h)
    {
        float xw = column * tileWidth;
        float yh = row * tileHeight;
        return { xw / texWidth, yh / texHeight, (xw + w) / texWidth, (yh + h) / texHeight };
    }

    Textures::~Textures()
    {
        cleanUp();
    }

    const Texture* Textures::addTexture(const std::string& id, const std::string& path, GLint minFilter, GLint magFilter)
    {
        return addTexture(id, path, 0, 0, 0, minFilter, magFilter);
    }

    const Texture* Textures::addTexture(const std::string& id, const std::string& path, int c, int w, int h, GLint minFilter, GLint magFilter)
    {
        auto mit = mTileSets.find(id);
        if (mit == mTileSets.end())
        {
            mTileSets[id] = Texture(id, c, w, h, loadTexture(path, minFilter, magFilter), path);
        }

        return &mTileSets[id];
    }

    void Textures::removeTexture(const std::string& id)
    {
        auto tex = mTileSets[id];
        mTileSets.erase(id);
        glDeleteTextures(1, &tex.texId);
    }

    void Textures::cleanUp()
    {
        for (auto& tm : mTileSets)
        {
            glDeleteTextures(1, &tm.second.texId);
        }
        mTileSets.clear();
    }

    GLTexture Textures::loadTexture(std::string filePath, GLint minFilter, GLint magFilter)
    {
        GLTexture texture = {};

        int BPP;
        uint8_t* out = stbi_load(filePath.c_str(), &texture.width, &texture.height, &BPP, 4);


        texture.id = createTexture(texture.width, texture.height, out, GL_RGBA8, GL_RGBA);
        //See https://en.wikipedia.org/wiki/Mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
        //unlink the texture
        glBindTexture(GL_TEXTURE_2D, 0);
        //delete the image buffer from memory
        if (out)
        {
            stbi_image_free(out);
        }
        else
        {
            std::printf("stbi fail: %s - %s\n", stbi_failure_reason(), filePath.c_str());
        }
        return texture;
    }
}