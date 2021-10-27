#pragma once

#include "Shapes.h"

#include <Math/Vectors.h>

namespace Plutus
{
    namespace Collider {
        // Circle Collision
        bool isColliding(Circle2d* circle1, Circle2d* circle2, MTV* mtv = nullptr);
        bool isColliding(Circle2d* circle, Line2d* line, MTV* mtv = nullptr);
        bool isColliding(Circle2d* circle, Box2d* box, MTV* mtv = nullptr);
        //Line Collision
        bool isColliding(Line2d* line1, Line2d* line2, MTV* mtv = nullptr);
        bool isColliding(Line2d* line, Circle2d* circle, MTV* mtv = nullptr);
        bool isColliding(Line2d* line, Box2d* box, MTV* mtv = nullptr);
        //Box Collision
        bool isColliding(Box2d* box1, Box2d* box2, MTV* mtv = nullptr);
        bool isColliding(Box2d* box, Circle2d* circle, MTV* mtv = nullptr);
        bool isColliding(Box2d* box, Line2d* line, MTV* mtv = nullptr);
    }
} // namespace Plutus