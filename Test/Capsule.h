#pragma once
#include <Physics/Shapes.h>

class b2Body;

namespace Plutus
{
    struct Capsule
    {
        float x;
        float y;
        float w;
        float h;
        b2Body* body;

        Capsule() = default;
        void create(float _x, float _y, float _w, float _h);

        Box2d getBox();
        Circle2d getBCircle();
        Circle2d getTCircle();

        void update();

    };
} // namespace Plutus