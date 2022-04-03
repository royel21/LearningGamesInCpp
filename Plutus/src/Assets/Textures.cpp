#include "Textures.h"
#include "stb_image.h"
#include <iostream>
#include <filesystem>
#include <cstdio>
#include <algorithm>
#include <Utils/Utils.h>

namespace Plutus
{
    Texture::Texture(const std::string& id, int c, int w, int h, GLTexture tex, const std::string& _path) : name(id), path(_path)
    {
        path = _path;

        columns = c;
        tileWidth = w;
        tileHeight = h;

        texId = tex.id;
        texWidth = tex.width;
        texHeight = tex.height;

        if (c)
        {
            calculateUV();
        }
        else
        {
            uvs.push_back({ 0, 0, 1, 1 });
        }
    }

    void Texture::calculateUV()
    {
        if (tileWidth > 0 && tileHeight > 0)
        {
            uvs.clear();
            auto totalTiles = columns * int(texHeight / tileHeight);

            for (int i = 0; i < totalTiles; i++)
            {
                int y = i / columns;
                int x = i % columns;
                vec4f UV;
                UV.x = ((float)(x * tileWidth) / (float)texWidth);
                UV.y = ((float)(y * tileHeight) / (float)texHeight);
                UV.z = ((float)(x * tileWidth + tileWidth) / (float)texWidth);
                UV.w = ((float)(y * tileHeight + tileHeight) / (float)texHeight);
                uvs.push_back(UV);
            }
        }
    }

    vec4f Texture::getUV(float column, float row, float w, float h)
    {
        float xw = column * tileWidth;
        float yh = row * tileHeight;
        return { xw / texWidth, yh / texHeight, (xw + w) / texWidth, (yh + h) / texHeight };
    }

    Textures::~Textures()
    {
        cleanUp();
    }

    Texture* Textures::getTexture(const std::string& id)
    {
        auto found = mTileSets.find(id);

        return found != mTileSets.end() ? &found->second : nullptr;
    }

    uint32_t Textures::getTextureId(const std::string& id)
    {
        auto found = mTileSets.find(id);

        return found != mTileSets.end() ? found->second.texId : 0;
    }

    vec4f Textures::getTextureUV(const std::string& id, int uvIndex)
    {
        auto found = mTileSets.find(id);
        return found != mTileSets.end() ? found->second.getUV(uvIndex) : vec4f(0, 0, 1, 1);
    }

    void Textures::remove(std::string texture)
    {
        auto it = mTileSets.find(texture);
        if (it != mTileSets.end()) {
            mTextureMap[it->second.path] = false;
            mTileSets.erase(it);
        }
    }

    Texture* Textures::addTexture(const std::string& id, const std::string& path, GLint minFilter, GLint magFilter)
    {
        return addTexture(id, path, 0, 0, 0, minFilter, magFilter);
    }

    Texture* Textures::addTexture(const std::string& id, const std::string& path, int c, int w, int h, GLint minFilter, GLint magFilter)
    {
        auto mit = mTileSets.find(id);
        if (mit == mTileSets.end())
        {
            mTileSets[id] = Texture(id, c, w, h, createGLTexure(path, minFilter, magFilter), path);
            return &mTileSets[id];
        }

        return &mit->second;
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

    GLTexture Textures::createGLTexure(std::string path, GLint minFilter, GLint magFilter) {

        GLTexture texture = {};

        int ch = Utils::getExtension(path).compare("png") == 0 ? 4 : 3;

        int BPP;
        uint8_t* out = stbi_load(path.c_str(), &texture.width, &texture.height, &BPP, ch);
        if (texture.width && texture.height) {
            auto format = ch == 3 ? GL_RGB8 : GL_RGBA8;
            auto type = ch == 3 ? GL_RGB : GL_RGBA;

            texture.id = createTexture(texture.width, texture.height, out, format, type, GL_UNSIGNED_BYTE, minFilter, magFilter);

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
                std::printf("stbi fail: %s - %s\n", stbi_failure_reason(), path.c_str());
            }
        }
        return texture;
    }
}