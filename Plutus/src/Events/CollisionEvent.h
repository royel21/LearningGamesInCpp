#pragma once
#include "IEvent.h"
#include <cstdint>

namespace Plutus
{
    enum CollisionType {
        COLLISIONSTART,
        COLLISIONEND
    };

    class CollisionEvent : public IEvent
    {
    public:
        uint32_t type = COLLISIONSTART;

        uint32_t entA;
        bool isEntASensor;
        uint32_t entB;
        bool isEntBSensor;

        CollisionEvent(uint32_t ent1, bool isSensorA, uint32_t ent2, bool isSensorB, uint32_t ctype = COLLISIONSTART) {
            entA = ent1;
            bool isEntASensor = isSensorA;
            entB = ent2;
            bool isEntBSensor = isSensorB;
            type = ctype;
        }
    };
} // namespace Plutus