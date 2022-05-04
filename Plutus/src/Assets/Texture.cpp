#include "Texture.h"
#include <stb_image.h>
#include <Utils/Utils.h>
#include <Utils/FileIO.h>
#include <Graphics/GLUtils.h>

namespace Plutus
{
    void Texture::init(const std::string& path, int w, int h, int minFilter, int magFilter)
    {
        destroy();

        mPath = path;

        if (minFilter && magFilter) {
            mMinFilter = minFilter;
            mMagFilter = magFilter;
        }

        mTileWidth = w;
        mTileHeight = h;
        loadTexture();
        (w && h) ? calculateUV() : uvs.push_back({ 0, 0, 1, 1 });
    }

    void Texture::setTilesSize(int w, int h)
    {
        if (w && h) {
            mTileWidth = w;
            mTileHeight = h;
            calculateUV();
        }
    }

    Vec4f Texture::getUVs(float column, float row, float w, float h)
    {
        float xw = column * mTileWidth;
        float yh = row * mTileHeight;
        return { xw / mWidth, yh / mHeight, (xw + w) / mWidth, (yh + h) / mHeight };
    }

    void Texture::destroy()
    {
        if (mTexId > -1)
            glDeleteTextures(1, &mTexId);
        mTexId = -1;
    }

    void Texture::calculateUV()
    {
        if (mTileWidth > 0 && mTileHeight > 0 && mHeight && mWidth)
        {
            uvs.clear();
            int spacingY = mSpacing;

            float tileWidth = (float)mTileWidth / (float)mWidth;
            float tileheight = (float)mTileHeight / (float)mHeight;

            Vec4f UV;
            for (int y = 0; y < mHeight / mTileHeight; y++)
            {
                UV.y = ((float)(y * mTileHeight + spacingY) / (float)mHeight);
                UV.w = UV.y + tileheight;

                int spacingX = mSpacing;
                for (int x = 0; x < mWidth / mTileWidth; x++)
                {

                    UV.x = ((float)(x * mTileWidth + spacingX) / (float)mWidth);
                    UV.z = UV.x + tileWidth;
                    uvs.push_back(UV);

                    spacingX += mSpacing;
                }

                spacingY += mSpacing;
            }
        }
    }

    void Texture::loadTexture()
    {
        int ch = Utils::getExtension(mPath).compare("png") == 0 ? 4 : 3;

        int BPP;
        uint8_t* out = stbi_load((baseDir + mPath).c_str(), &mWidth, &mHeight, &BPP, ch);
        if (mWidth && mHeight) {
            auto format = ch == 3 ? GL_RGB8 : GL_RGBA8;
            auto gltype = ch == 3 ? GL_RGB : GL_RGBA;

            mTexId = createTexture(mWidth, mHeight, out, format, gltype, GL_UNSIGNED_BYTE, mMinFilter, mMagFilter);
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