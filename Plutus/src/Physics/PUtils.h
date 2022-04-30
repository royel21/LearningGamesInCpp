#pragma once

#include <Math/Vectors.h>

#include "Shapes.h"

using Points = std::vector<Plutus::Vec2f>;

namespace Plutus
{
    namespace PUtils {
        Vec2f ClosestPointLineCircle(Circle2d* c, Line2d* line);
        bool PointInBox(Vec2f& p, Box2d* box);
        bool PointInLine(Vec2f& p, Line2d* line);
        bool PointInCircle(Vec2f& p, Circle2d* circle);
        Vec2f getProjection(const Points& verts, Vec2f axis);
        bool TestVertices(const Points& verts1, const Points& verts2, MTV* mtv = nullptr);
        bool TestVertices(const Points& verts1, const Points& verts2, const Points axices, MTV* mtv = nullptr);
    }
} // namespace Plutus
