#include "PickingFrameBuffer.h"
#include <Graphics/GLheaders.h>

namespace Plutus
{
    PickingFrameBuffer::~PickingFrameBuffer()
    {
        glDeleteFramebuffers(1, &mFbId);
        glDeleteTextures(1, &mTexId);
        glDeleteRenderbuffers(1, &mRbufferId);
    }

    void PickingFrameBuffer::init(int w, int h)
    {
        //Grenerate a frame buffer
        glGenFramebuffers(1, &mFbId);
        //bind the buffer for imediate use
        glBindFramebuffer(GL_FRAMEBUFFER, mFbId);
        //Create the texture
        mTexId = createTexture(w, h, 0, GL_RGB32F, GL_RGB, GL_FLOAT);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexId, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            assert("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    u32 PickingFrameBuffer::getEntId(glm::vec2 pos)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFbId);

        glReadBuffer(GL_COLOR_ATTACHMENT0);
        float pixels[] = { 0,0,0 };
        glReadPixels(static_cast<int>(pos.x), static_cast<int>(pos.y), 1, 1, GL_RGB, GL_FLOAT, pixels);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return (u32)pixels[0] - 1;
    }

    void PickingFrameBuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFbId);
        glDisable(GL_BLEND);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void PickingFrameBuffer::unBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glEnable(GL_BLEND);
    }
}