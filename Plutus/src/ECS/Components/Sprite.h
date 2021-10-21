#pragma once

#include <string>
#include <glm/glm.hpp>
#include <Assets/AssetManager.h>

namespace Plutus
{
    struct Sprite
    {
        std::string mTextureId;
        bool mFlipX = false;
        bool mFlipY = false;
        glm::vec4 mUVCoord;
        uint32_t mColor = -1;

        Sprite() {};
        Sprite(std::string texId) : mTextureId(texId), mUVCoord(0, 0, 1, 1) {};
    };
} // namespace Plutus