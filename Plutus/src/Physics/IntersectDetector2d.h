#pragma once
#include "Primitives.h"

#include "Line.h"

#include <glm/glm.hpp>

namespace Plutus
{
    class IntersectDetector2d
    {
    public:
        static bool pointOnLine(const glm::vec2& point, const Line& line);

        static bool pointInCircle(const glm::vec2& point, const Circle& circle);

        static bool pointAABB(const glm::vec2& point, const AABB& aabb);
    };
} // namespace Plutus