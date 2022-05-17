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
    struct Rect {
        Vec2f pos;
        Vec2f size;
        Rect() {};
        Rect(float x, float y, float w, float h) : pos(x, y), size(w, h) {}
        Rect(const Vec2f& p, const Vec2f& s) : pos(p), size(s) {}

        inline bool interset(const Vec2f& p) { return p >= pos && p <= (pos + size); }

        bool overlap(const Rect& p) {
            return
                interset(p.pos) ||
                interset(p.pos + p.size) ||
                interset({ p.pos.x, p.pos.y + p.size.y }) ||
                interset({ p.pos.x + p.size.x, p.pos.y });
        }

        bool contain(const Rect& rect) {
            return rect.pos > pos && rect.pos + rect.size < pos + size;
        }

        Vec4f getBox() { return { pos, size }; }
    };

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
        Shader mShader;
        SpriteBatch2D mSpritebatch;
        PlutusEngine::SpriteBatch mSpriteBatch2;

        std::vector<ColorRect> rects;
        Limiter mLimiter;
        Rect rect1;
    };
} // namespace Plutus