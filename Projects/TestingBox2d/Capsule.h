#pragma once
#include <Physics/Shapes.h>

class b2Body;

namespace Plutus
{
    struct Capsule
    {
        b2Body* body;

        Vec2f position;
        Vec2f size;
        float radius;

        Capsule() = default;
        void create(float _x, float _y, float _w, float _h);

        Box2d getBox();
        Circle2d getBCircle();
        Circle2d getTCircle();

        void update();

    };
} // namespace Plutus