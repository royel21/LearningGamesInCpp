#pragma once
#include "Primitives.h"

#include "Line.h"

#include <glm/glm.hpp>

namespace Plutus
{
    class IntersectDetector2d
    {
    public:
        static bool pointOnLine(const glm::vec2& point, Line& line);

        static bool pointInCircle(const glm::vec2& point, Circle& circle);

        static bool pointAABB(const glm::vec2& point, AABB& aabb);

        static bool pointBox(const glm::vec2& point, Box2d& box);

        static bool lineInCircle(Line line, Circle circle);
    };
} // namespace Plutus