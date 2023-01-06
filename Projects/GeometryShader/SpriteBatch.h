#pragma once

#include "RenderBatch.h"
#include "Vertex.h"

#include <vector>
#include <Math/Vectors.h>
#include <Utils/ColorRGBA8.h>
#include <string>

namespace Plutus
{
    struct Project;
    struct Renderable;

    struct BatchTex {
        uint32_t texId = 0;
        uint32_t texUnit = 0;
        uint32_t vertCount = 0;
        uint32_t iboOffset = 0;
        BatchTex() {}
    };

    struct SpriteVert : Vertex {
        ColorRGBA8 color;
        SpriteVert(float x, float y, float ux, float uy, ColorRGBA8 c) : color(c), Vertex(x, y, ux, uy) {}
        SpriteVert(const Vec2f& v, float ux, float uy, ColorRGBA8 c) : color(c), Vertex(v.x, v.y, ux, uy) {}
    };

    class SpriteBatch : public RenderBatch
    {
    public:
        SpriteBatch() {}
        SpriteBatch(Camera2D* camera, Shader* shader = nullptr) : RenderBatch(camera, shader) {  };
        ~SpriteBatch() { destroy(); }

        void init(uint32_t MAX_SPRITE = 60000);
        void addSprite(Renderable* renderable);
        void addText(const std::string& fontId, float x, float y, const std::string& text, ColorRGBA8 color = {}, float scale = 1);
        void draw(Shader* shader = nullptr) override;
        void destroy() override;

    private:
        uint32_t offset = 0;
        std::vector<BatchTex> mBatches;
        std::vector<SpriteVert> sprites;
        const uint32_t mVertexSize = sizeof(SpriteVert);

        void createBatch(uint32_t texId, uint32_t texUnit);

    };
} // namespace Plutus