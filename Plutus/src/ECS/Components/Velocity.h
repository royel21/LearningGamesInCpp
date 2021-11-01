#pragma once
#include <Math/Vectors.h>

namespace Plutus
{
    struct Velocity
    {
        vec2f mVelocity;
        Velocity() = default;
        Velocity(float x, float y) : mVelocity(x, y) {};

        void setVel(float x, float y) { mVelocity = { x, y }; }
    };
} // namespace Plutus
