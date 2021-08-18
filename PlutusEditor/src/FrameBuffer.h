#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "types.h"
#include "glm/glm.hpp"

class FrameBuffer
{
public:
    FrameBuffer() {};
    ~FrameBuffer();
    void init(int width, int height, bool reset = false);
    void bind();
    void unBind();
    inline void resize(const glm::ivec2& size)
    {
        isDirty = true;
        mSize = size;
    }
    inline void resize(int w, int h)
    {
        isDirty = true;
        mSize = { w, h };
    }

    glm::vec2 getSize() { return mSize; }

    float getAspectRatio() { return static_cast<float>(mSize.x) / static_cast<float>(mSize.y); }

    void setDirty() { isDirty = true; };

    uint32 getTextureId() const
    {
        return textId;
    }

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

#endif