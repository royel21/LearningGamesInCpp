#pragma once

#include "RenderBatch.h"
#include "Vertex.h"

#include <vector>
#include <Math/Vectors.h>
#include <Utils/ColorRGBA8.h>


namespace Plutus
{
    enum ShapeType {
        CIRCLE = 0,
        ROUNDRECT
    };

    struct Project;
    struct Shape;

    struct ShapeVert : Vertex {
        ColorRGBA8 color;
        float radius;
        float type = 0;
        ShapeVert(float x, float y, float ux, float uy, ColorRGBA8 c, float r, float t) : color(c), radius(r), type(t), Vertex(x, y, ux, uy) {}
    };

    class ShapeBatch : public RenderBatch
    {
    public:
        ShapeBatch() {}
        ShapeBatch(Camera2D* camera, Shader* shader = nullptr) : RenderBatch(camera, shader) {  };
        ~ShapeBatch() { destroy(); }

        void init(uint32_t MAX_SPRITE = 60000);
        void addSprite(const Vec4f& rect, ColorRGBA8 c, float radius, int type = CIRCLE);
        void draw(Shader* shader = nullptr) override;
        void destroy() override;

    private:
        Vec4f mUv = { 0,0,1,1 };
        uint32_t offset = 0;
        std::vector<ShapeVert> mShapes;
        const uint32_t mVertexSize = sizeof(ShapeVert);

        void createBatch(int type);

    };
} // namespace Plutus