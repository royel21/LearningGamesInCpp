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
    struct Texture : Asset
    {
    public:
        Texture() = default;
        Texture(const std::string& path, int w = 0, int h = 0, GLint glFilter = GL_NEAREST) {
            init(path, w, h, glFilter);
        }

        void init(const std::string& path, int w = 0, int h = 0, GLint glFilter = GL_NEAREST);

        void setTilesSize(int w, int h);

        ~Texture() { destroy(); }

        inline Vec4f getUVs(int uvIndex)
        {
            return uvs.size() && uvIndex < (int)uvs.size() ? uvs[uvIndex] : Vec4f(0, 0, 1, 1);
        }

        Vec4f getUVs(float column, float row, float w, float h);

        void destroy() override;

        void calculateUV();

        template<typename ...TArgs>
        inline Vec4f getUV(TArgs &&... args) {
            return getUVs(std::forward<TArgs>(args)...);
        }

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
        uint32_t mTexId = -1;
        std::vector<Vec4f> uvs;
    };
} // namespace Plutus