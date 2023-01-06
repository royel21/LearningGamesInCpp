#include "PUtils.h"

#include <Math/PMath.h>

namespace Plutus
{
    namespace PUtils {
        Vec2f ClosestPointLineCircle(Circle2d* circle, Line2d* line)
        {
            Points points = line->getVertices();
            auto p1 = points[0];
            auto p2 = points[1];

            // distance between line points
            auto lineDist = (p1 - p2);
            // length square of the line
            auto lengthSqtr = lineDist.lengthSqrt();

            // unit vector in the direction of the line
            auto dist = lineDist.unit();

            float dot = dist.dot(((p1 - circle->pos)));

            // if point out of the line in the negative side of the axis return start point because is the closest point
            if (dot > lengthSqtr) {
                return p2;
            }
            // if point out of the line in the positive side of the axis return end point because is the closest point
            if (dot < 0) {
                return p1;
            }
            return p1 - (dist * dot);
        }

        bool PointOnLine(Vec2f& point, Line2d* line)
        {
            float dy = line->size.y - line->pos.y;
            float dx = line->size.x - line->pos.x;
            if (dx == 0) {
                return PMath::compareF(point.x, line->pos.x);
            }

            float m = dy / dx;
            float b = line->size.y - (m * line->size.x);

            //check the line equation
            return point.y == m * point.x + b;
        }

        bool PointInBox(Vec2f& point, Box2d* box)
        {
            Vec2f p = point;
            if (box->rotation) {
                PMath::rotate(p, box->getCenter(), box->rotation);
            }
            return p >= box->pos && p <= box->getMax();
        }


        bool PointInCircle(Vec2f& p, Circle2d* c)
        {
            return (p - c->pos).lengthSqrt() - 1 < c->radiusSqrt();
        }


        Vec2f getProjection(const Points& verts, Vec2f axis)
        {
            Vec2f result;

            result.x = axis.dot(verts[0]);
            result.y = result.x;
            for (const Vec2f& v : verts) {
                float proj = axis.dot(v);

                if (proj < result.x) result.x = proj;
                if (proj > result.y) result.y = proj;
            }
            return result;
        }

        bool TestVertices(const Points& verts1, const Points& verts2, const Points axices, MTV* mtv) {

            for (auto& axis : axices) {
                auto  proj1 = getProjection(verts1, axis);
                auto  proj2 = getProjection(verts2, axis);
                if (proj1.x >= proj2.y || proj2.x >= proj1.y) {
                    return true;
                }
            }
            return false;
        }

        bool TestVertices(const Points& verts1, const Points& verts2, MTV* mtv) {
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
