#pragma once
#include <cstdint>

namespace Plutus
{
    class ICollisionListener
    {
    public:
        virtual void CollisionEvent(uint32_t ent1, bool isSensorA, uint32_t ent2, bool isSensorB, bool collisionStart = true) = 0;
    };
} // namespace Plutus