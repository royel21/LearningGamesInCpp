#include "FrameBuffer.h"
#include <Graphics/GLheaders.h>
#include <assert.h>

namespace Plutus
{
    void FrameBuffer::init(int w, int h, bool isForPicking)
    {
        mIsPicking = isForPicking;
        mSize = { w, h };
        if (isDirty)
        {
            isDirty = false;
            cleanUp();
        }
        //Grenerate a frame buffer
        glGenFramebuffers(1, &mFbId);
        //bind the buffer for imediate use
        glBindFramebuffer(GL_FRAMEBUFFER, mFbId);

        if (!isForPicking) {
            mTexId = createTexture(w, h, 0, GL_RGB, GL_RGB);
            // Create render buffer for store the depth info
            glGenRenderbuffers(1, &mRbufferId);
            glBindRenderbuffer(GL_RENDERBUFFER, mRbufferId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h); // use a single renderbuffer object for both a depth AND stencil buffer.
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRbufferId);
        }
        else {
            mTexId = createTexture(w, h, 0, GL_RGB32F, GL_RGB, GL_FLOAT);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexId, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            assert("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

        glViewport(0, 0, w, h);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    FrameBuffer::~FrameBuffer()
    {
        cleanUp();
    }

    u32 FrameBuffer::getEntId(glm::vec2 pos)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mFbId);

        glReadBuffer(GL_COLOR_ATTACHMENT0);
        float pixels[] = { 0,0,0 };
        glReadPixels(static_cast<int>(pos.x), static_cast<int>(pos.y), 1, 1, GL_RGB, GL_FLOAT, pixels);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return (u32)pixels[0] - 1;
    }
    void FrameBuffer::bind()
    {
        if (isDirty)
            init(mSize.x, mSize.y);

        glBindFramebuffer(GL_FRAMEBUFFER, mFbId);
        if (mIsPicking) {
            glDisable(GL_BLEND);
            glClearColor(0, 0, 0, 0);
        }
        else {
            glClearColor(mBGColor.x, mBGColor.y, mBGColor.z, mBGColor.w);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void FrameBuffer::unBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (mIsPicking) {
            glEnable(GL_BLEND);
        }
    }

    void FrameBuffer::cleanUp()
    {
        glDeleteFramebuffers(1, &mFbId);
        glDeleteTextures(1, &mTexId);
        glDeleteRenderbuffers(1, &mRbufferId);
    }

} // namespace Plutus
