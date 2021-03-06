#pragma once
#include "System.h"

#include <Graphics/Shader.h>
#include <Graphics/SpriteBatch2D.h>

namespace Plutus
{
    class FrameBuffer;

    class RenderSystem : public ISystem
    {
    public:
        RenderSystem(Camera2D* camera);
        ~RenderSystem() { destroy(); }
        void init(Project* project) override;
        void update(float dt) override;
        void destroy() override;
        inline void setFrameBuffer(FrameBuffer* fbuffer) { mFrameBuff = fbuffer; }
        SpriteBatch2D* getSpritebath() { return &mRenderer; }

    private:
        Shader mShader;
        SpriteBatch2D mRenderer;
        FrameBuffer* mFrameBuff = nullptr;
        std::vector<Renderable> mRenderables;
    };
} // namespace Plutus
