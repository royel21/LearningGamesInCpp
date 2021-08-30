#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "types.h"
#include "glm/glm.hpp"
namespace Plutus
{

    class FrameBuffer
    {
    public:
        FrameBuffer() = default;
        ~FrameBuffer();
        void init(int width, int height);
        void bind();
        void unBind();

        void resize(const glm::ivec2 &size)
        {
            isDirty = true;
            mSize = size;
        }

        void resize(int w, int h)
        {
            isDirty = true;
            mSize = {w, h};
        }

        glm::vec2 getSize() { return mSize; }

        float getAspectRatio() { return static_cast<float>(mSize.x) / static_cast<float>(mSize.y); }

        void setDirty() { isDirty = true; };

        uint32 getTextureId() const { return textId; }

        void cleanUp();

    private:
        //Framebuffer Id
        uint32 fboId = 0;
        //texture Id
        uint32 textId = 0;
        //renderbuffer Id
        uint32 rboId = 0;
        bool isDirty = false;
        glm::ivec2 mSize;
    };
} // namespace Plutus

#endif