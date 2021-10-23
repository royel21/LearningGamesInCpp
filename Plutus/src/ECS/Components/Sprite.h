#pragma once

#include <string>
#include <Math/Vectors.h>

namespace Plutus
{
    struct Sprite
    {
        std::string mTextureId;
        bool mFlipX = false;
        bool mFlipY = false;
        uint32_t mColor = -1;
        vec4f mUVCoord;

        Sprite() {};
        Sprite(std::string texId) : mTextureId(texId), mUVCoord(0, 0, 1, 1) {};
    };
} // namespace Plutus