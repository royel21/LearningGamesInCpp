#include "FrameBuffer.h"
#include <glad/glad.h>
#include <assert.h>

namespace Plutus
{
    void FrameBuffer::init(int w, int h)
    {
        mSize = { w, h };
        if (isDirty)
        {
            isDirty = false;
            cleanUp();
        }
        //Grenerate a frame buffer
        glGenFramebuffers(1, &fboId);
        //bind the buffer for imediate use
        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
        //Genenerate a texture for draw our buffer
        glGenTextures(1, &textId);
        glBindTexture(GL_TEXTURE_2D, textId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        //See https://en.wikipedia.org/wiki/Mipmap
        glGenerateMipmap(GL_TEXTURE_2D);
        //attach the buffer to the texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textId, 0);
        // Create render buffer for store the depth info
        glGenRenderbuffers(1, &rboId);
        glBindRenderbuffer(GL_RENDERBUFFER, rboId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboId);

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

    void FrameBuffer::bind()
    {
        if (isDirty)
            init(mSize.x, mSize.y);

        glBindFramebuffer(GL_FRAMEBUFFER, fboId);
        glBindTexture(GL_TEXTURE_2D, textId);
    }

    void FrameBuffer::unBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void FrameBuffer::cleanUp()
    {
        glDeleteFramebuffers(1, &fboId);
        glDeleteTextures(1, &textId);
        glDeleteRenderbuffers(1, &rboId);
    }

} // namespace Plutus
