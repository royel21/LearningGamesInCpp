#pragma once
#include <vector>
#include <Math/Vectors.h>
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
    enum ShapeType {
        PLine = 0,
        PCircle,
        PBox
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

    struct Line2d : public Shape {
        vec2f end;
        float rotation = 0;
        Line2d() { type = PLine; };
        Line2d(float x1, float y1, float x2, float y2, float r = 0);
        Line2d(const vec2f& _start, const vec2f& _end, float r = 0);

        vec2f getDir() { return (end - pos).unit(); }
    };

    struct Circle2d : public Shape
    {
        float radius = 1.0f;
        Circle2d() { type = PCircle; };
        Circle2d(float x, float y, float r) : Shape(x, y), radius(r) { type = PCircle; }
        Circle2d(const vec2f& _pos, float r) : Shape(_pos), radius(r) { type = PCircle; }

        float radiusSqrt() const { return radius * radius; }
    };

    struct Box2d : public Shape
    {
        vec2f size;
        vec2f half;
        float rotation = 0;
        Box2d() { type = PBox; };
        Box2d(float x, float y, float w, float h, float r = 0);
        Box2d(const vec2f& pos, const vec2f& _size, float r = 0);

        vec2f getLocalMin() const { return pos - half; }
        vec2f getLocalMax() const { return pos + half; }
        vec2f getMax() const { return pos + size; }
        vec2f getCenter() const { return pos + half; }
        std::vector<vec2f> getVertices();
    };
}