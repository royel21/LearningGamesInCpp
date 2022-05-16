#pragma once
#include <Core/Window.h>

#include <Graphics/Shader.h>
#include <Graphics/Camera2D.h>
#include <Graphics/SpriteBatch2D.h>
#include "SpriteBatch.h"

#include <vector>
#include <Time/Limiter.h>

namespace Plutus
{
    struct ColorRect {
        Vec4f rect;
        PlutusEngine::ColorRGBA8 color;

        ColorRect(const Vec4f& r, const PlutusEngine::ColorRGBA8& c) : rect(r), color(c) {};
    };

    class App
    {
    public:
        App() = default;
        ~App() = default;
        void init();
        void update();
        void draw();
        void run();

    private:
        Window mWindow;
        Camera2D mCamera;
        Shader mShader;
        SpriteBatch2D mSpritebatch;
        PlutusEngine::SpriteBatch mSpriteBatch2;

        std::vector<ColorRect> rects;
        Limiter mLimiter;

    };
} // namespace Plutus