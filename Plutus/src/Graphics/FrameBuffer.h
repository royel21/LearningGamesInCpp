#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <glm/glm.hpp>
#include <Math/Vectors.h>

namespace Plutus
{

    class FrameBuffer
    {
    public:
        FrameBuffer() : mBGColor(0) {};
        ~FrameBuffer();
        void init(int width, int height, bool isForPicking = false);
        void bind();
        void unBind();

        int getEntId(glm::vec2 pos);

        void resize(const glm::ivec2& size)
        {
            isDirty = true;
            mSize = size;
        }

        void resize(int w, int h)
        {
            isDirty = true;
            mSize = { w, h };
        }

        glm::vec2 getSize() { return mSize; }

        float getAspectRatio() { return static_cast<float>(mSize.x) / static_cast<float>(mSize.y); }

        void setDirty() { isDirty = true; };

        uint32_t getTextureId() const { return mTexId; }

        void setColor(glm::vec4 bgColor) { mBGColor = bgColor; }
        void setColor(vec4f bgColor) { mBGColor = { bgColor.x,bgColor.y,bgColor.z,bgColor.w }; }

        void cleanUp();
    private:
        //Framebuffer Id
        uint32_t mFbId = 0;
        //texture Id
        uint32_t mTexId = 0;
        //renderbuffer Id
        uint32_t mRbufferId = 0;
        bool isDirty = false;
        glm::ivec2 mSize;

        glm::vec4 mBGColor;

        bool mIsPicking = false;
    };
} // namespace Plutus

#endif