#pragma once

#include "Asset.h"

#include <vector>

#include <Math/Vectors.h>
#include <Graphics/GLheaders.h>

namespace Plutus
{
    /*
     *   Texture Replesenting a single Texture or a Spritesheet
     *
     *   @param Path Path in asset folder
     *   @param TileWith default = 0
     *   @param TileHeight default = 0
     *   @param minFilter Minify Filter default = GL_NEAREST
     *   @param magFilter Magnify Filter default = GL_NEAREST
    */
    struct TileSet {
        float columns;
        float width;
        float height;
        float spacingX;
        float spacingY;
    };

    struct Texture : Asset
    {
    public:
        Texture() = default;
        Texture(const std::string& path, int w = 0, int h = 0, GLint glFilter = GL_NEAREST, int texture = 0) {
            init(path, w, h, glFilter);
        }

        void init(const std::string& path, int w = 0, int h = 0, GLint glFilter = GL_NEAREST, int texture = 0);

        void setTilesSize(int w, int h);

        ~Texture() { destroy(); }

        inline Vec4f getUV(int uvIndex)
        {
            return uvIndex < (int)uvs.size() ? uvs[uvIndex] : Vec4f(0, 0, 1, 1);
        }

        Vec4f getUV(float column, float row, float w, float h);

        void destroy() override;

        void calculateUV();

    private:
        void loadTexture();

    public:
        int mTileWidth = 0;
        int mTileHeight = 0;
        int mGlFilter = GL_NEAREST;
        int mWidth = 0;
        int mHeight = 0;
        int mSpacing = 0;
        int mMargin = 0;
        int mTexureUnit = 0;
        uint32_t mTexId = -1;
        std::vector<Vec4f> uvs;
        TileSet mTileSet;
    };
} // namespace Plutus