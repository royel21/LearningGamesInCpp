#pragma once
#include <vector>
#include "Vectors.h"
#include <box2d/b2_math.h>

constexpr float PIXEL_SIZE = 100.0f;
//Pixel Per Meter
constexpr float PPM = 1 / PIXEL_SIZE;
//Meter Per Pixel
constexpr float MPP = PIXEL_SIZE;
//Half Meter Per Pixel
constexpr float HMPP = PPM / 2.0f;

class b2Body;

namespace Plutus
{
    using Points = std::vector<vec2f>;

    enum ShapeType {
        Line = 0,
        Circle,
        Box
    };


    inline b2Vec2 toWorld(const vec2f& value) {
        return { value.x * PPM, value.y * PPM };
    }

    inline vec2f fromWorld(b2Vec2 value) {
        return { value.x * MPP, value.y * MPP };
    }

    inline b2Vec2 tobVec2(const vec2f& vec) { return { vec.x, vec.y }; }

    struct Shape {
        vec2f pos;
        ShapeType type;
        b2Body* body;

        Shape() = default;
        Shape(float x, float y) : pos(x, y) {}
        Shape(const vec2f _pos) : pos(_pos) {}

        virtual void update();
    };

    struct Line2d {
        vec2f start;
        vec2f end;
        vec2f dir;
        Line2d() = default;
        Line2d(float x1, float y1, float x2, float y2) : start(x1, y1), end(x2, y2) {
            dir = (end - start).unit();
        }
        Line2d(const vec2f& _start, const vec2f& _end) {
            start = _start;
            end = _end;
            dir = (end - start).unit();
        }

        Points getPoints() {
            return { vec2f{ start.x, start.y }, vec2f{ end.x, end.y } };
        }
    };

    struct Circle2d : public Shape
    {
        float radius = 1.0f;
        Circle2d() = default;
        Circle2d(float x, float y, float r) : Shape(x, y), radius(r) {}
        Circle2d(const vec2f& _pos, float r) : Shape(_pos), radius(r) {}
    };

    struct Box2d : public Shape
    {
        vec2f size;
        vec2f half;
        Box2d() = default;
        Box2d(float x, float y, float w, float h) : Shape(x, y), size(w, h) { half = { w * 0.5f, h * 0.5f }; }
        Box2d(const vec2f& pos, const vec2f& _size) : Shape(pos), size(size) { half = size * 0.5f; }
    };
}