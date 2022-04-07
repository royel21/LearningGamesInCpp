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
     *   @param TextureID Asset Id
     *   @param Path Path in asset folder
     *   @param TileCount default = 0
     *   @param TileWith default = 0
     *   @param TileHeight default = 0
     *   @param minFilter Minify Filter default = GL_NEAREST
     *   @param magFilter Magnify Filter default = GL_NEAREST
    */
    struct Texture : Asset
    {
    public:
        Texture() = default;
        Texture(const std::string& path, int c = 0, int w = 0, int h = 0, GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);

        ~Texture() { destroy(); }

        inline vec4f getUVs(int uvIndex)
        {
            return uvs.size() && uvIndex < (int)uvs.size() ? uvs[uvIndex] : vec4f(0, 0, 1, 1);
        }

        vec4f getUVs(float column, float row, float w, float h);

        void destroy() override;

        void calculateUV();

        template<typename ...TArgs>
        vec4f getUV(TArgs &&... args) {
            return getUVs(std::forward<TArgs>(args)...);
        }

    private:
        void loadTexture(GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);

    public:
        int mColumns = 0;
        int mTileWidth = 0;
        int mTileHeight = 0;
        int mWidth = 0;
        int mHeight = 0;
        uint32_t mTexId = 0;
        std::vector<vec4f> uvs;
    };
} // namespace Plutus