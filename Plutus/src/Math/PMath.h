#pragma once

#include <limits>

#include "Vectors.h"

namespace Plutus
{
    void rotate(vec2f& point, const vec2f& Origin, float angle);

    bool compareF(float x, float y, float epsilon = std::numeric_limits<float>::min());

    bool compare(const vec2f& v1, const vec2f& v2, float epsilon = std::numeric_limits<float>::min());
} // namespace Plutus