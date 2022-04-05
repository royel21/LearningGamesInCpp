#pragma once

#include <string>
#include <Math/Vectors.h>

namespace Plutus
{
    struct SpriteComponent
    {
        std::string mTextureId;
        bool mFlipX = false;
        bool mFlipY = false;
        uint32_t mColor = -1;
        vec4f mUVCoord;

        SpriteComponent() {};
        SpriteComponent(std::string texId) : mTextureId(texId), mUVCoord(0, 0, 1, 1) {};
    };
} // namespace Plutus