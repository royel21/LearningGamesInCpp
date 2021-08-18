#pragma once
#include "Serialize/Serialize.h"

namespace Plutus
{
    struct Velocity
    {
        float x;
        float y;
        Velocity(float _x, float _y) : x(_x), y(_y) {};
    };

    inline void Velocity_JSON(Serializer& serializer, const Velocity& v) {
        auto writer = serializer.getWriter();
        writer->StartObject();
        {
            writer->String("name");
            writer->String("Velocity");
            writer->String("x");
            writer->Double(v.x);
            writer->String("y");
            writer->Double(v.y);
        }
    }
} // namespace Plutus
