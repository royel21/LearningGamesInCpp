#pragma once
#include <vector>
#include "Vectors.h"


namespace Plutus
{
    using Points = std::vector<vec2f>;

    struct Line {
        vec2f start;
        vec2f end;
        vec2f dir;
        Line() = default;
        Line(float x1, float y1, float x2, float y2) : start(x1, y1), end(x2, y2) {
            dir = (end - start).unit();
        }
        Line(const vec2f& _start, const vec2f& _end) {
            start = _start;
            end = _end;
            dir = (end - start).unit();
        }
        Points getPoints() {
            return { vec2f{ start.x, start.y }, vec2f{ end.x, end.y } };
        }
    };

    struct Circle
    {
        vec2f pos;
        float radius = 1.0f;
        Circle() = default;
        Circle(float x, float y, float r) : pos({ x, y }), radius(r) {}
        Circle(const vec2f& _pos) : pos(_pos) {}
    };

    struct Box2d
    {
        vec2f pos;
        vec2f size;
        vec2f half;

        Box2d() { half = size * 0.5f; }

        Box2d(const vec2f& _pos, const vec2f& _size)
        {
            size = _size;
            pos = _pos;
            half = size * 0.5f;
        }

        vec2f center() {
            return pos + half;
        }
    };
}