#pragma once
#include <Core/Window.h>

#include <Graphics/Shader.h>
#include <Graphics/Camera2D.h>
#include <Graphics/SpriteBatch2D.h>

#include <vector>
#include <Time/Limiter.h>

#include "QuadTree.h"

namespace Plutus
{
    struct ColorRect {
        Rect rect;
        ColorRGBA8 color;

        ColorRect(const Rect& r, const ColorRGBA8& c) : rect(r), color(c) {};
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
        SpriteBatch2D mSpritebatch;

        std::vector<ColorRect> rects;
        QuadTreeContainer<ColorRect> mQTrees;
        uint32_t count;
        Limiter mLimiter;
        Rect rect1;
    };
} // namespace Plutus