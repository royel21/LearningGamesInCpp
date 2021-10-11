#pragma once

#include "Shapes.h"
#include "Vectors.h"

namespace Plutus
{
    class Collider
    {
    public:
        static bool pointOnLine(const vec2f& point, Line& line);

        static bool pointInCircle(const vec2f& point, Circle& circle);

        static bool pointBox(const vec2f& point, Box2d& box);

        static bool lineInCircle(Line line, Circle circle);
    };
} // namespace Plutus