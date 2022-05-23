#pragma once

#include "RenderBatch.h"
#include "Vertex.h"

#include <vector>
#include <Math/Vectors.h>
#include <Utils/ColorRGBA8.h>


namespace Plutus
{
    struct Project;
    struct Renderable;

    struct SpriteVert : Vertex {
        ColorRGBA8 color;
        SpriteVert(float x, float y, float ux, float uy, ColorRGBA8 c) : color(c), Vertex(x, y, ux, uy) {}
    };

    class SpriteBatch : public RenderBatch
    {
    public:
        SpriteBatch() {}
        SpriteBatch(Camera2D* camera, Shader* shader = nullptr) : RenderBatch(camera, shader) {  };
        ~SpriteBatch() { destroy(); }

        void init(uint32_t MAX_SPRITE = 60000);
        void addSprite(Renderable* renderable);
        void draw(Shader* shader = nullptr) override;
        void destroy() override;

    private:
        Vec2f mtopLeft;
        Vec2f mbottomLeft;
        Vec2f mtopRight;
        Vec2f mBottomRight;
        uint32_t offset = 0;
        std::vector<BatchTex> mBatches;
        std::vector<SpriteVert> sprites;
        const uint32_t mVertexSize = sizeof(SpriteVert);

        void createBatch(Texture* tex);

    };
} // namespace Plutus