#include "batcher.h"

#include <Graphics/Graphic.h>

namespace Plutus
{
    void Batcher::init()
    {
        mVAO = Graphic::createVertexArray();
        mBuffId = Graphic::createBufferArray();
    }
} // namespace Plutus
