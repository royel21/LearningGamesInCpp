#pragma once

#include "RenderBatch.h"
#include <Utils/ColorRGBA8.h>

#include <vector>
#include <Math/Vectors.h>

namespace Plutus
{
    struct Project;

    struct SpriteVert {
        float x;
        float y;
        float uvx;
        float uvy;
        ColorRGBA8 color;
    };

    class SpriteBatch : public RenderBatch
    {
    public:
        SpriteBatch(Camera2D* camera, Shader* shader) : RenderBatch(camera, shader) {  };
        ~SpriteBatch() { destroy(); }

        void init();
        void addSprite(uint32_t texId, const Vec4f& rect, Vec4f& uv = { 0,0,1,1 }, ColorRGBA8 c = {}, bool flipx = false, bool flipy = false, float rotation = 0);
        void draw() override;
        void destroy() override;

    private:
        std::vector<SpriteVert> sprites;
        Vec2f mtopLeft;
        Vec2f mbottomLeft;
        Vec2f mtopRight;
        Vec2f mBottomRight;

    };
} // namespace Plutus