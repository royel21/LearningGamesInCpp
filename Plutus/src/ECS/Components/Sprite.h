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
        int mUvIndex = 0;
        glm::vec4 mUVCoord;
        uint32_t mColor;

        Sprite() = default;
        Sprite(std::string texId) : mTextureId(texId), mUVCoord(0, 0, 1, 1) {};

        GLuint getTexId() { return AssetManager::get()->getTexId(mTextureId); }
        glm::vec4 getUV() { return AssetManager::get()->getTexCoords(mTextureId, mUvIndex); }
    };
} // namespace Plutus