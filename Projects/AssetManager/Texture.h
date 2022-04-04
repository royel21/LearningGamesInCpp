#pragma once

#include "Asset.h"

#include <vector>

#include <Math/Vectors.h>
#include <Graphics/GLheaders.h>

namespace Plutus
{
    struct Texture2 : Asset
    {
    public:
        Texture2(const std::string& path, int c = 0, int w = 0, int h = 0, GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);

        inline vec4f getUV(int uvIndex)
        {
            return uvs.size() && uvIndex < (int)uvs.size() ? uvs[uvIndex] : vec4f(0, 0, 1, 1);
        }

        vec4f getUV(float column, float row, float w, float h);

        void destroy() override;

    private:
        void calculateUV();
        void loadTexture(GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);

    public:
        std::string mPath;
        int mColumns = 0;
        int mTileWidth = 0;
        int mTileHeight = 0;
        int mWidth = 0;
        int mHeight = 0;
        uint32_t mTexId = 0;
        std::vector<vec4f> uvs;
    };
} // namespace Plutus