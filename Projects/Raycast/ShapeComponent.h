#pragma once
#include <vector>
#include <Math/Vectors.h>

namespace Plutus
{
    struct ShapeComponent
    {
        Vec2f pos;
        std::vector<Vec2f> Points;
    };
}// End Plutus namespace