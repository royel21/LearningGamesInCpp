#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <Math/Vectors.h>

namespace Plutus
{

    class FrameBuffer
    {
    public:
        FrameBuffer() : mBGColor(1) {};
        ~FrameBuffer();
        void init(int width, int height, bool isForPicking = false);
        void bind();
        void unBind();

        int getEntId(vec2f pos);

        void resize(const vec2i& size)
        {
            isDirty = true;
            mSize = size;
        }

        void resize(int w, int h)
        {
            isDirty = true;
            mSize = { w, h };
        }

        vec2f getSize() { return { mSize.x, mSize.y }; }

        float getAspectRatio() { return static_cast<float>(mSize.x) / static_cast<float>(mSize.y); }

        void setDirty() { isDirty = true; };

        unsigned int getTextureId() const { return mTexId; }

        void setColor(vec4f bgColor) { mBGColor = { bgColor.x,bgColor.y,bgColor.z,bgColor.w }; }

        void cleanUp();
    private:
        //Framebuffer Id
        unsigned int mFbId = 0;
        //texture Id
        unsigned int mTexId = 0;
        //renderbuffer Id
        unsigned int mRbufferId = 0;
        bool isDirty = false;
        vec2i mSize;

        vec4f mBGColor;

        bool mIsPicking = false;
    };
} // namespace Plutus

#endif