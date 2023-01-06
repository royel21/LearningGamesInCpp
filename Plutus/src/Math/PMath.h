#pragma once

#include <limits>

#include "Vectors.h"

#define CLAMP(v, min, max) v > min ? min : v > max ? max : v

namespace Plutus
{
    namespace PMath {
        constexpr float PI = 3.14159265359f;

        constexpr float DEC2RA(float dec) { return dec * (PI / 180.0f); }

        void rotate(Vec2f& point, const Vec2f& Origin, float angle);

        Vec2f rotateP(const Vec2f& point, const Vec2f& Origin, float angle);

        bool compareF(float x, float y, float epsilon = std::numeric_limits<float>::min());

        bool compare(const Vec2f& v1, const Vec2f& v2, float epsilon = std::numeric_limits<float>::min());

        Vec2f max(const Vec2f& a, float y);

        Vec2f round(const Vec2f& v);

        float invSqrt(float n);
    }
} // namespace Plutus