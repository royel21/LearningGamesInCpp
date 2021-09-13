#pragma once

#include <cmath>
#include <glm/glm.hpp>

constexpr float PI = 3.14159265359f;

namespace Plutus
{
    glm::vec2 rotatePoint2D(const glm::vec2& point, float angle);
} // namespace Plutus
