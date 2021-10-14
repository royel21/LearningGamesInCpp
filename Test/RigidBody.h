#pragma once
#include <Physics/Vectors.h>
#include <box2d/b2_body.h>

namespace Plutus
{
    class RigidBody
    {
    private:
        b2Body* body;
    public:
        RigidBody();


    };
} // namespace Plutus