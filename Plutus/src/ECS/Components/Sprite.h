#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Graphics/vertex.h"

namespace Plutus
{
    class Sprite
    {
    public:
        GLuint mTexId = 0;
        bool mFlipX = false;
        bool mFlipY = false;
        glm::vec4 mUVCoord;
        ColorRGBA8 mColor;
        std::string mTextureId;

    public:
        Sprite();
        Sprite(std::string textId);
    };
} // namespace Plutus