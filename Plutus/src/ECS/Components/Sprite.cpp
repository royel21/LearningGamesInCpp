#include "Sprite.h"
#include "Assets/Textures.h"

namespace Plutus
{
    Sprite::Sprite() : mUVCoord(0, 0, 1, 1), mTextureId(0)
    {
    }

    Sprite::Sprite(std::string textId) : mUVCoord(0, 0, 1, 1), mTextureId(textId)
    {
        mTexId = Textures::get()->getTexture(textId)->texId;
    }
} // namespace Plutus