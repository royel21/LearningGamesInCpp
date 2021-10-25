#include "Shapes.h"
#include <box2d/b2_body.h>

#include <Math/PMath.h>

namespace Plutus
{
    void Shape::update() {
        if (type != PLine)
            pos = fromWorld(body->GetPosition());
    }

    Line2d::Line2d(float x1, float y1, float x2, float y2, float r) : end(x2, y2) {
        rotation = r;
        pos = { x1, y1 };
        type = PLine;
    }

    Line2d::Line2d(const vec2f& _start, const vec2f& _end, float r) {
        rotation = r;
        type = PLine;
        pos = _start;
        end = _end;
    }

    Box2d::Box2d(float x, float y, float w, float h, float r) : Shape(x, y), size(w, h)
    {
        type = PBox;
        rotation = r;
        half = { w * 0.5f, h * 0.5f };
    }

    Box2d::Box2d(const vec2f& pos, const vec2f& _size, float r) : Shape(pos), size(size)
    {
        type = PBox;
        rotation = r;
        half = size * 0.5f;
    }

    std::vector<vec2f> Box2d::getVertices() {

        auto min = pos;
        auto max = getMax();

        std::vector<vec2f> vertices{ {min}, {min.x, max.y}, {max}, {max.x, min.y} };
        if (rotation != 0.0f) {
            for (auto& vert : vertices) {
                rotate(vert, pos, rotation);
            }
        }

        return vertices;
    }
} // namespace Plutus
