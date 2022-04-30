#pragma once

#include <string>
#include <Math/Vectors.h>
#include <Utils/ColorRGBA8.h>

namespace Plutus
{
    struct SpriteComponent
    {
        std::string mTextureId;
        bool mFlipX = false;
        bool mFlipY = false;
        ColorRGBA8 mColor;
        Vec4f mUVCoord;

        SpriteComponent() {};
        SpriteComponent(std::string texId, ColorRGBA8 color = {}) : mTextureId(texId), mColor(color), mUVCoord(0, 0, 1, 1) {};
    };
} // namespace Plutus