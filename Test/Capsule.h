#pragma once
#include <Physics/Shapes.h>

namespace Plutus
{
    struct Capsule
    {
        float x;
        float y;
        float w;
        float h;

        Capsule() = default;
        void create(float _x, float _y, float _w, float _h);
        Box2d getBox();
        Circle2d getBCircle();
        Circle2d getTCircle();

    };
} // namespace Plutus