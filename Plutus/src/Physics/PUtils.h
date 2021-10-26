#pragma once

#include <Math/Vectors.h>

#include "Shapes.h"

using Points = std::vector<Plutus::vec2f>;

namespace Plutus
{

    //Min Translation Vector for SAT separation
    struct MTV {
        float dist;
        vec2f axis;
    };

    namespace PUtils {
        vec2f ClosestPointLineCircle(Circle2d* c, Line2d* line);
        bool PointInBox(vec2f& p, Box2d* box);
        bool PointInLine(vec2f& p, Line2d* line);
        bool PointInCircle(vec2f& p, Circle2d* circle);
        vec2f getProjection(const Points& verts, vec2f axis);
        bool TestVertices(const Points& verts1, const Points& verts2, MTV* mtv = nullptr);
        bool TestVertices(const Points& verts1, const Points& verts2, const Points axices, MTV* mtv = nullptr);
    }
} // namespace Plutus
