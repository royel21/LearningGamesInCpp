#pragma once
#include <vector>

#include <Box2d/box2d.h>
#include <Physics/Shapes.h>

#include "Shapes.h"

class b2Body;

#pragma once

namespace Plutus
{
    class RigidBody
    {
        bool fixedRotation;

        b2Body* body;
        std::vector<Shape*> shapes;

        ~RigidBody();
    };
} // namespace Plutus