#pragma once
#include <cstdint>
#include <Graphics/IndexBuffer.h>

namespace Plutus
{
    struct Texture;
    class Shader;
    class Camera2D;

    class RenderBatch {
    protected:
        uint32_t mVAO;
        uint32_t mIboId;
        uint32_t mBufferId;

        uint8_t mLayer;
    public:
        Camera2D* mCamera;
        Shader* mShader;

    public:
        RenderBatch(Camera2D* camera = nullptr, Shader* shader = nullptr) : mCamera(camera), mShader(shader) {}
        virtual void draw(Shader* shader = nullptr) = 0;
        virtual void destroy() {}

        inline void setShader(Shader* shader) { mShader = shader; }
        inline void setCamera(Camera2D* camera) { mCamera = camera; }

        bool operator < (const RenderBatch& t) const {
            return mLayer < t.mLayer;
        }

        bool operator < (const RenderBatch* t) const {
            return mLayer < t->mLayer;
        }
    };

} // namespace Plutus