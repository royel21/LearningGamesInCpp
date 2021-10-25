#pragma once

#include "Shapes.h"

#include <Math/Vectors.h>

namespace Plutus
{
    class Collider
    {
    public:
        static bool pointOnLine(vec2f& point, Line2d& line);
        static bool pointInCircle(vec2f& point, Circle2d& circle);
        static bool pointBox(vec2f& point, Box2d& box);
        // Circle Collision
        static bool isColliding(Circle2d& circle1, Circle2d& circle2);
        static bool isColliding(Circle2d& circle, Line2d& line);
        static bool isColliding(Circle2d& circle, Box2d& box);
        //Line Collision
        static bool isColliding(Line2d& line1, Line2d& line2);
        static bool isColliding(Line2d& line, Circle2d& circle);
        static bool isColliding(Line2d& line, Box2d& box);
        //Box Collision
        static bool isColliding(Box2d& box1, Box2d& box2);
        static bool isColliding(Box2d& box, Circle2d& circle);
        static bool isColliding(Box2d& box, Line2d& line);
    };
} // namespace Plutus