#include "PUtils.h"

#include <Math/PMath.h>

namespace Plutus
{
    namespace PUtils {
        vec2f ClosestPointLineCircle(Circle2d* circle, Line2d* line)
        {
            Points points = line->getVertices();
            auto p1 = points[0];
            auto p2 = points[1];
            auto unit = (p2 - p1).unit();

            auto line2Circle = p1 - circle->pos;

            if (unit.dot(line2Circle) > 0) {
                return p1;
            }

            line2Circle = circle->pos - p2;
            if (unit.dot(line2Circle) > 0) {
                return p2;
            }

            auto closestDist = unit.dot(p1 - circle->pos);

            auto closestVec = unit * closestDist;

            return p1 - closestVec;
        }

        bool PointOnLine(vec2f& point, Line2d* line)
        {
            float dy = line->end.y - line->pos.y;
            float dx = line->end.x - line->pos.x;
            if (dx == 0) {
                return compareF(point.x, line->pos.x);
            }

            float m = dy / dx;
            float b = line->end.y - (m * line->end.x);

            //check the line equation
            return point.y == m * point.x + b;
        }

        bool PointInBox(vec2f& point, Box2d* box)
        {
            vec2f p = point;
            if (box->rotation) {
                rotate(p, box->getCenter(), box->rotation);
            }
            return p >= box->pos && p <= box->getMax();
        }


        bool PointInCircle(vec2f& p, Circle2d* c)
        {
            return (p - c->pos).lengthSqrt() - 1 < c->radiusSqrt();
        }


        vec2f getProjection(const Points& verts, vec2f axis)
        {
            vec2f result;

            result.x = axis.dot(verts[0]);
            result.y = result.x;
            for (const vec2f& v : verts) {
                float proj = axis.dot(v);

                if (proj < result.x) result.x = proj;
                if (proj > result.y) result.y = proj;
            }
            return result;
        }

        bool TestVertices(const Points& verts1, const Points& verts2, const Points axices) {

            for (auto& axis : axices) {
                auto  proj1 = getProjection(verts1, axis);
                auto  proj2 = getProjection(verts2, axis);
                if (proj1.x >= proj2.y || proj2.x >= proj1.y) {
                    return true;
                }
            }
            return false;
        }

        bool TestVertices(const Points& verts1, const Points& verts2) {
            auto size = verts1.size();
            for (size_t i = 0; i < size; i++) {
                auto axis = (verts1[(i + 1) % size] - verts1[i]).normal();

                auto  proj1 = getProjection(verts1, axis);
                auto  proj2 = getProjection(verts2, axis);
                if (proj1.x >= proj2.y || proj2.x >= proj1.y) {
                    return true;
                }
            }
            return false;
        }
    }
} // namespace Plutus
