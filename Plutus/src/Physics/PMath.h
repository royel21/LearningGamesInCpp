#pragma once

#include <limits>
#include <glm/glm.hpp>

namespace Plutus
{
    void rotate(glm::vec2& point, const glm::vec2& Origin, float angle);

    bool compareF(float x, float y, float epsilon = std::numeric_limits<float>::min());

    bool compare(const glm::vec2& v1, const glm::vec2& v2, float epsilon = std::numeric_limits<float>::min());
} // namespace Plutus