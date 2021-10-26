#pragma once

#include "Shapes.h"

#include <Math/Vectors.h>

namespace Plutus
{
    struct Manifold {
        Points forces;
    };
    namespace Collider {
        // Circle Collision
        bool isColliding(Circle2d* circle1, Circle2d* circle2, Manifold* manifold = nullptr);
        bool isColliding(Circle2d* circle, Line2d* line, Manifold* manifold = nullptr);
        bool isColliding(Circle2d* circle, Box2d* box, Manifold* manifold = nullptr);
        //Line Collision
        bool isColliding(Line2d* line1, Line2d* line2, Manifold* manifold = nullptr);
        bool isColliding(Line2d* line, Circle2d* circle, Manifold* manifold = nullptr);
        bool isColliding(Line2d* line, Box2d* box, Manifold* manifold = nullptr);
        //Box Collision
        bool isColliding(Box2d* box1, Box2d* box2, Manifold* manifold = nullptr);
        bool isColliding(Box2d* box, Circle2d* circle, Manifold* manifold = nullptr);
        bool isColliding(Box2d* box, Line2d* line, Manifold* manifold = nullptr);
    }
} // namespace Plutus