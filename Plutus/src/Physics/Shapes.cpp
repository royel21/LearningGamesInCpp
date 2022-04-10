#include "Shapes.h"
#include <box2d/b2_body.h>
#include <box2d/b2_math.h>

#include <Math/PMath.h>

namespace Plutus
{
    void Shape::update() {
        if (type != EdgeShape)
            pos = fromWorld(body->GetPosition());
    }

    Line2d::Line2d(float x1, float y1, float x2, float y2, float r) : end(x2, y2) {
        rotation = r;
        pos = { x1, y1 };
        type = EdgeShape;
    }

    Line2d::Line2d(const vec2f& _start, const vec2f& _end, float r) {
        rotation = r;
        type = EdgeShape;
        pos = _start;
        end = _end;
    }

    vec2f Line2d::getCenter() {
        return (pos + end) * 0.5;
    }

    Points Line2d::getVertices() {
        Points vertices{ pos, end };
        if (rotation != 0.0f) {
            for (auto& vert : vertices) {
                rotate(vert, getCenter(), rotation);
            }
        }

        if (lastRotation != rotation) {
            axes.push_back((vertices[1] - vertices[0]).normal().unit());
            lastRotation = rotation;
        }

        return vertices;
    }

    Box2d::Box2d(float x, float y, float w, float h, float r) : Shape(x, y), size(w, h)
    {
        type = BoxShape;
        rotation = r;
        half = { w * 0.5f, h * 0.5f };

        getVertices();
    }

    Box2d::Box2d(const vec2f& pos, const vec2f& _size, float r) : Shape(pos), size(_size)
    {
        type = BoxShape;
        rotation = r;
        half = size * 0.5f;

        getVertices();
    }

    Points Box2d::getVertices() {
        auto min = pos;
        auto max = getMax();
        Points vertices{ {min}, {max.x, min.y}, {max}, {min.x, max.y} };
        if (rotation) {
            for (auto& vert : vertices) {
                rotate(vert, getCenter(), rotation);
            }
        }

        if (lastRotation != rotation) {
            axes.clear();
            axes.push_back((vertices[1] - vertices[0]).normal().unit());
            axes.push_back((vertices[2] - vertices[1]).normal().unit());
            lastRotation = rotation;
        }
        return vertices;
    }
} // namespace Plutus
