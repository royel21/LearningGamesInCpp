#pragma once

#include "RenderBatch.h"
#include "Vertex.h"

#include <vector>
#include <Math/Vectors.h>
#include <Utils/ColorRGBA8.h>

#include <Graphics/Shader.h>


namespace Plutus
{
    enum ShapeType {
        CIRCLE = 0,
        ROUNDRECT
    };

    struct Project;
    struct Shape;

    struct ShapeVert : Vertex {
        Vec2f size;
        ColorRGBA8 color;
        float radius = 0;
        int type = 0;
        ShapeVert(const Vec2f& p, float ux, float uy, const Vec2f& s, ColorRGBA8 c, float r = 0, int t = 0) : size(s), color(c), radius(r), type(t), Vertex(p.x, p.y, ux, uy) {}
    };

    class ShapeBatch
    {
    public:
        ShapeBatch() {}
        ~ShapeBatch() { destroy(); }

        void init(Camera2D* camera);
        inline void addRect(const Vec4f& rect, float radius = 0, ColorRGBA8 c = {}) { addShape(rect, c, radius, ROUNDRECT); }
        inline void addCircle(const Vec2f& pos, float radius, ColorRGBA8 c = {}) { addShape({ pos - radius * .5f, radius, radius }, c); }
        void draw();
        void destroy();

    private:
        Shader mShader;
        Camera2D* mCamera;
        uint32_t mVAO;
        IndexBuffer mIbo;
        uint32_t mBufferId;

        Vec4f mUv = { -1,-1,1,1 };
        uint32_t mIndexCount = 0;

        std::vector<ShapeVert> mShapes;
        const uint32_t mVertexSize = sizeof(ShapeVert);

        void addShape(const Vec4f& rect, ColorRGBA8 c = {}, float radius = 0, int type = CIRCLE);
    };
} // namespace Plutus