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
        //Genenerate a texture for draw our buffer
        glGenTextures(1, &mTexId);
        glBindTexture(GL_TEXTURE_2D, mTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
        //See https://en.wikipedia.org/wiki/Mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
        //attach the buffer to the texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexId, 0);
        // Create render buffer for store the depth info
        glGenRenderbuffers(1, &mRbufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, mRbufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRbufferId);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            assert("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    u32 PickingFrameBuffer::getEntId(glm::vec2 pos)
    {
        enable();
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        float pixels[] = { 0,0,0 };
        glReadPixels(static_cast<int>(pos.x), static_cast<int>(pos.y), 1, 1, GL_RGB, GL_FLOAT, pixels);
        return (u32)pixels[0] - 1;
        disable();
    }

    void PickingFrameBuffer::enable()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFbId);
    }

    void PickingFrameBuffer::disable()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
}