#include "Texture.h"
#include <iostream>
#include <filesystem>
#include <cstdio>
#include <algorithm>
#include <stb_image.h>
#include <Utils/Utils.h>

namespace Plutus
{
    Texture::Texture(const std::string& path, int c, int w, int h, GLint minFilter, GLint magFilter)
    {
        mPath = path;

        mColumns = c;
        mTileWidth = w;
        mTileHeight = h;
        loadTexture(minFilter, magFilter);
        c ? calculateUV() : uvs.push_back({ 0, 0, 1, 1 });
    }

    vec4f Texture::getUVs(float column, float row, float w, float h)
    {
        float xw = column * mTileWidth;
        float yh = row * mTileHeight;
        return { xw / mWidth, yh / mHeight, (xw + w) / mWidth, (yh + h) / mHeight };
    }

    void Texture::destroy()
    {
        glDeleteTextures(1, &mTexId);
    }

    void Texture::calculateUV()
    {
        if (mTileWidth > 0 && mTileHeight > 0)
        {
            uvs.clear();
            auto totalTiles = mColumns * int(mHeight / mTileHeight);

            for (int i = 0; i < totalTiles; i++)
            {
                int y = i / mColumns;
                int x = i % mColumns;
                vec4f UV;
                UV.x = ((float)(x * mTileWidth) / (float)mWidth);
                UV.y = ((float)(y * mTileHeight) / (float)mHeight);
                UV.z = ((float)(x * mTileWidth + mTileWidth) / (float)mWidth);
                UV.w = ((float)(y * mTileHeight + mTileHeight) / (float)mHeight);
                uvs.push_back(UV);
            }
        }
    }

    void Texture::loadTexture(GLint minFilter, GLint magFilter)
    {
        int ch = Utils::getExtension(mPath).compare("png") == 0 ? 4 : 3;

        int BPP;
        uint8_t* out = stbi_load(mPath.c_str(), &mWidth, &mHeight, &BPP, ch);
        if (mWidth && mHeight) {
            auto format = ch == 3 ? GL_RGB8 : GL_RGBA8;
            auto gltype = ch == 3 ? GL_RGB : GL_RGBA;

            mTexId = createTexture(mWidth, mHeight, out, format, gltype, GL_UNSIGNED_BYTE, minFilter, magFilter);
            //unlink the texture
            glBindTexture(GL_TEXTURE_2D, 0);
            //delete the image buffer from memory
            if (out)
            {
                stbi_image_free(out);
            }
            else
            {
                std::printf("stbi fail: %s - %s\n", stbi_failure_reason(), mPath.c_str());
            }
        }
    }
}