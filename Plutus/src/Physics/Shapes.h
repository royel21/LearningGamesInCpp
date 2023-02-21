#pragma once
#include <vector>
#include <Math/Vectors.h>

#include <Math/Rect.h>
#include <Utils/ColorRGBA8.h>

class b2Body;

constexpr int EdgeShape = 0;
constexpr int CircleShape = 1;
constexpr int BoxShape = 2;


namespace Plutus
{
    using Points = std::vector<Vec2f>;

    //Min Translation Vector for SAT separation
    struct MTV {
        float dist;
        Vec2f axis;
    };

    struct Shape {
        Vec2f pos;
        Vec2f size;
        float radius = 1.0f;
        ColorRGBA8 color;

        int type;
        b2Body* body;

        Points axes;
        Points vertices;
        bool isStatic = false;

        Shape() = default;
        Shape(float x, float y): pos(x, y) {}
        Shape(const Vec2f _pos): pos(_pos) {}
        Rect getRect();

        virtual void update();
    };

    struct Line2d: public Shape {
        float rotation = 0;

        Line2d() { type = EdgeShape; };
        Line2d(float x1, float y1, float x2, float y2, float r = 0);
        Line2d(const Vec2f& _start, const Vec2f& _end, float r = 0);

        Vec2f getCenter();
        Points getVertices();

    private:
        float lastRotation = -1;
    };

    struct Circle2d: public Shape
    {
        Circle2d() { type = CircleShape; };
        Circle2d(float x, float y, float r): Shape(x, y) { radius = r; type = CircleShape; }
        Circle2d(const Vec2f& _pos, float r): Shape(_pos) { radius = r; type = CircleShape; }

        float radiusSqrt() const { return radius * radius; }
    };

    struct Box2d: public Shape
    {
        Vec2f half;
        Points axes;
        float rotation = 0;

        Box2d() { type = BoxShape; };
        Box2d(float x, float y, float w, float h, float r = 0);
        Box2d(const Vec2f& pos, const Vec2f& _size, float r = 0);

        Vec2f getMax() const { return pos + size; }
        Vec2f getCenter() const { return pos + half; }

        Points getVertices();

    private:
        float lastRotation = -1;
    };
}