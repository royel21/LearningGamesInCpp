#pragma once
#include <Math/Vectors.h>

namespace Plutus
{
    struct VelocityComponent
    {
        vec2f mVelocity;
        VelocityComponent() = default;
        VelocityComponent(float x, float y) : mVelocity(x, y) {};

        void setVel(float x, float y) { mVelocity = { x, y }; }
    };
} // namespace Plutus
