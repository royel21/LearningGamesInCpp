#pragma once

#include "Shapes.h"

#include <Math/Vectors.h>

namespace Plutus
{
    namespace Collider {
        // Circle Collision
        bool isColliding(Circle2d& circle1, Circle2d& circle2);
        bool isColliding(Circle2d& circle, Line2d& line);
        bool isColliding(Circle2d& circle, Box2d& box);
        //Line Collision
        bool isColliding(Line2d& line1, Line2d& line2);
        bool isColliding(Line2d& line, Circle2d& circle);
        bool isColliding(Line2d& line, Box2d& box);
        //Box Collision
        bool isColliding(Box2d& box1, Box2d& box2);
        bool isColliding(Box2d& box, Circle2d& circle);
        bool isColliding(Box2d& box, Line2d& line);
    }
} // namespace Plutus