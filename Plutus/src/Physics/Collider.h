#pragma once

#include "Shapes.h"

#include <Math/Vectors.h>

namespace Plutus
{
    class Collider
    {
    public:
        static bool pointOnLine(const vec2f& point, Line2d& line);

        static bool pointInCircle(const vec2f& point, Circle2d& circle);

        static bool pointBox(const vec2f& point, Box2d& box);

        static bool lineInCircle(Line2d line, Circle2d circle);
    };
} // namespace Plutus