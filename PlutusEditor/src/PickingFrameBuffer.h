#pragma once
#include "types.h"
#include "glm/glm.hpp"

namespace Plutus
{
    class PickingFrameBuffer
    {
    public:
        PickingFrameBuffer() = default;
        ~PickingFrameBuffer();

        void init(int w, int h);
        u32 getEntId(glm::vec2 pos);
        void enable();
        void disable();
    private:
        //Frame Buffer Id
        u32 mFbId;
        //texture Id
        u32 mTexId;
        //Render Buffer Id
        u32 mRbufferId;
    };
} // namespace Plutus