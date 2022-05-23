#pragma once
#include <cstdint>
#include <Graphics/IndexBuffer.h>

namespace Plutus
{
    struct Texture;
    class Shader;
    class Camera2D;

    struct BatchTex {
        Texture* texture = nullptr;
        uint32_t vertCount = 0;
        uint32_t iboOffset = 0;
        BatchTex() {}
    };

    class RenderBatch {
    protected:
        uint32_t mVAO;
        uint32_t mBufferId;
        uint8_t mLayer;
        IndexBuffer mIbo;
    public:
        Camera2D* mCamera;
        Shader* mShader;

    public:
        RenderBatch(Camera2D* camera, Shader* shader = nullptr) : mCamera(camera), mShader(shader) {}
        virtual void draw() = 0;
        virtual void destroy() {}

        bool operator < (const RenderBatch& t) const {
            return mLayer < t.mLayer;
        }

        bool operator < (const RenderBatch* t) const {
            return mLayer < t->mLayer;
        }
    };

} // namespace Plutus