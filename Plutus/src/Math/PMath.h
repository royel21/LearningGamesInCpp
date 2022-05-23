#pragma once

#include <limits>

#include "Vectors.h"

namespace Plutus
{
    constexpr float PI = 3.14159265359f;

    constexpr float DEC2RA(float dec) { return dec * (PI / 180.0f); }

    void rotate(Vec2f& point, const Vec2f& Origin, float angle);

    bool compareF(float x, float y, float epsilon = std::numeric_limits<float>::min());

    bool compare(const Vec2f& v1, const Vec2f& v2, float epsilon = std::numeric_limits<float>::min());

    float invSqrt(float n);
} // namespace Plutus