#include "Texture.h"
#include <stb_image.h>
#include <Utils/Utils.h>
#include <Utils/FileIO.h>
#include <Graphics/GLUtils.h>

#include <Log/Logger.h>

namespace Plutus
{
    void Texture::init(const std::string& path, int w, int h, int glFilter, int texture)
    {
        mTexureUnit = texture;
        destroy();

        mPath = path;

        if (glFilter) mGlFilter = glFilter;

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
            mTileSet.columns = float(mWidth / mTileWidth);
            mTileSet.width = float(mTileWidth) / float(mWidth);
            mTileSet.height = float(mTileHeight) / float(mHeight);
            mTileSet.spacingX = float(mSpacing) / float(mWidth);
            mTileSet.spacingY = float(mSpacing) / float(mHeight);

            uvs.clear();
            Vec4f UV;
            for (int y = 0; y < mHeight / mTileHeight; y++)
            {
                UV.y = (y * mTileSet.height) + (mTileSet.spacingY * (y + 1));
                UV.w = UV.y + mTileSet.height - mTileSet.spacingY;

                for (int x = 0; x < mWidth / mTileWidth; x++)
                {
                    UV.x = (x * mTileSet.width) + (mTileSet.spacingX * (x + 1));
                    UV.z = UV.x + mTileSet.width - mTileSet.spacingX;
                    uvs.push_back(UV);
                }
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
            glActiveTexture(GL_TEXTURE0 + mTexureUnit);
            mTexId = createTexture(mWidth, mHeight, out, format, gltype, GL_UNSIGNED_BYTE, mGlFilter, mGlFilter);
            //unlink the texture
            glBindTexture(GL_TEXTURE_2D, 0);
            //delete the image buffer from memory
            if (out)
            {
                stbi_image_free(out);
            }
            else
            {
                Logger::info("stbi fail: %s - %s\n", stbi_failure_reason(), mPath.c_str());
            }
            glActiveTexture(GL_TEXTURE0);
        }
    }
}