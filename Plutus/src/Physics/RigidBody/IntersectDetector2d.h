#pragma once
#include "Primitives.h"

#include <glm/glm.hpp>
#include <Physics/Line.h>

namespace Plutus
{
    class IntersectDetector2d
    {
    public:
        static bool pointOnLine(const glm::vec2& point, Line line);

        static bool pointInCircle(glm::vec2 point, Circle circle);
    };
} // namespace Plutus