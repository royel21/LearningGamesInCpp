#pragma once

#include <limits>

#include "Vectors.h"

namespace Plutus
{
    void rotate(Vec2f& point, const Vec2f& Origin, float angle);

    bool compareF(float x, float y, float epsilon = std::numeric_limits<float>::min());

    bool compare(const Vec2f& v1, const Vec2f& v2, float epsilon = std::numeric_limits<float>::min());

    float invSqrt(float n);
} // namespace Plutus