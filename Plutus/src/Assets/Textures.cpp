#include "Textures.h"
#include "stb_image.h"
#include <iostream>
#include <filesystem>
#include <cstdio>
#include <algorithm>

namespace Plutus
{
    TileSet::TileSet(const std::string &id, int c, int w, int h, GLTexture tex, const std::string &_path) : name(id), mTexture(tex), path(_path)
    {
        path = _path;
        tileWidth = w;
        tileHeight = h;
        columns = c;
        totalTiles = 0;
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
            uvs.push_back({0, 0, 1, 1});
        }
    }

    void TileSet::calculateUV()
    {
        uint32_t w = mTexture.width;
        uint32_t h = mTexture.height;

        if (tileWidth > 0)
        {
            mTexCount = columns * int(h / tileHeight);
            for (int i = 0; i < mTexCount; i++)
            {
                int y = i / columns;
                int x = i % columns;
                glm::vec4 UV;
                UV.x = ((float)(x * tileWidth) / (float)w);
                UV.y = ((float)(y * tileHeight + tileHeight) / (float)h);
                UV.z = ((float)(x * tileWidth + tileWidth) / (float)w);
                UV.w = ((float)(y * tileHeight) / (float)h);
                uvs.push_back(UV);
            }
        }
    }
    Textures *Textures::get()
    {
        static Textures textures;
        return &textures;
    }

    Textures::~Textures()
    {
        cleanUp();
    }

    const TileSet *Textures::addTexture(const std::string &id, const std::string &path, GLint minFilter, GLint magFilter)
    {
        return addTexture(id, path, 0, 0, 0, minFilter, magFilter);
    }

    const TileSet *Textures::addTexture(const std::string &id, const std::string &path, int c, int w, int h, GLint minFilter, GLint magFilter)
    {
        auto mit = mTileSets.find(path);
        if (mit == mTileSets.end())
        {
            mTileSets[id] = TileSet(id, c, w, h, loadTexture(path, minFilter, magFilter), path);
        }

        return &mTileSets[id];
    }

    void Textures::removeTexture(const std::string &id)
    {
        auto tex = mTileSets[id];
        mTileSets.erase(id);
        glDeleteTextures(1, &tex.mTexture.id);
    }

    void Textures::cleanUp()
    {
        for (auto &tm : mTileSets)
        {
            glDeleteTextures(1, &tm.second.mTexture.id);
        }
        mTileSets.clear();
    }

    GLTexture Textures::loadTexture(std::string filePath, GLint minFilter, GLint magFilter)
    {
        GLTexture texture = {};
#ifndef __EMSCRIPTEN__
        std::filesystem::path path = filePath.c_str();
        filePath = std::filesystem::absolute(path).string();
#endif
        int BPP;
        uint8_t *out = stbi_load(filePath.c_str(), &texture.width, &texture.height, &BPP, 4);

        glGenTextures(1, &texture.id);
        //link the image to a texture in the gpu texture array

        glBindTexture(GL_TEXTURE_2D, texture.id);
        //flag to render the image
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        //Load the image to the memory of the gpu
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, out);

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