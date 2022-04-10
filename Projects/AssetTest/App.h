#pragma once
#include <Core.h>
#include <Graphics/Shader.h>
#include <Graphics/SpriteBatch2D.h>

namespace Plutus
{
    class DebugRender;
    class App : public Core
    {
    public:
        App(const char* name, int width, int height) : Core(name, width, height) {}

        void Setup() override;

        void Update(float) override;

        void Resize(int w, int h)override;
        //Called on the loop draw, should be overrided.
        void Draw() override;

        void Exit() override;
    private:
        Shader mShader;
        SpriteBatch2D mRenderer;
        DebugRender* mDebug;
    };
} // namespace Plutus
