#pragma once

#include <string>
#include <glm/glm.hpp>
#include <Graphics/vertex.h>
#include <Assets/AssetManager.h>

namespace Plutus
{
    class Sprite
    {
    public:
        std::string mTextureId;
        bool mFlipX = false;
        bool mFlipY = false;
        glm::vec4 mUVCoord;
        ColorRGBA8 mColor;

    public:
        Sprite();
        Sprite(std::string texId) : mTextureId(texId), mUVCoord(0, 0, 1, 1) {};

        GLuint getTexId() { return AssetManager::get()->getTexId(mTextureId); }
    };
} // namespace Plutus