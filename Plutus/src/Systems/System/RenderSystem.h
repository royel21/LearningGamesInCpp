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
        void init() override;
        void update(float dt) override;
        void draw() override;
        void destroy() override;
        inline void setFrameBuffer(FrameBuffer* fbuffer) { mFrameBuff = fbuffer; }
        SpriteBatch2D* getSpritebath() { return &mRenderer; }

    private:
        SpriteBatch2D mRenderer;
        FrameBuffer* mFrameBuff = nullptr;
        std::vector<Renderable> mRenderables;
    };
} // namespace Plutus
