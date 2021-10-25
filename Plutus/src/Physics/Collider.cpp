#include "Collider.h"
#include "PUtils.h"

#include <Math/PMath.h>

#define CLAMPVAL(v, min, max) v < min ? min : v > max ? max : v

namespace Plutus
{
    namespace Collider {

        //Circle Vs Circle
        bool isColliding(Circle2d& c1, Circle2d& c2)
        {
            float r = c1.radius + c2.radius;
            return (c2.pos - c1.pos).lengthSqrt() - 1 <= r * r;
        }
        // Circle Vs Line
        bool isColliding(Circle2d& circle, Line2d& line)
        {
            if (PUtils::PointInCircle(line.pos, circle) || PUtils::PointInCircle(line.end, circle)) {
                return true;
            }

            auto lineSeg = line.end - line.pos;

            auto centerToLineStart = circle.pos - line.pos;

            float t = centerToLineStart.dot(lineSeg) / lineSeg.dot(lineSeg);
            // t is a percentage of the line and can't be smaller or bigger of the line size
            if (t < 0.0f || t > 1.0f) {
                return false;
            }

            //find the point close to the segment
            auto closestPoint = line.pos + (lineSeg * t);

            return PUtils::PointInCircle(closestPoint, circle);
        }

        bool rotateBox(Circle2d& circle, Box2d& box) {
            vec2f min;
            vec2f max = box.half * 2;

            auto c2boxCenter = circle.pos - box.pos;
            rotate(c2boxCenter, min, -box.rotation);

            auto localCirclePos = c2boxCenter + box.half;

            auto closest = localCirclePos;

            closest.x = CLAMPVAL(closest.x, min.x, max.x);
            closest.y = CLAMPVAL(closest.y, min.y, max.y);

            auto circle2box = localCirclePos - closest;

            return circle2box.lengthSqrt() <= circle.radiusSqrt();
        }
        // Circle Vs Box
        bool isColliding(Circle2d& circle, Box2d& box)
        {
            if (box.rotation) {
                return rotateBox(circle, box);
            }

            else {
                auto min = box.pos;
                auto max = box.getMax();
                auto closest = circle.pos;

                closest.x = CLAMPVAL(closest.x, min.x, max.x);
                closest.y = CLAMPVAL(closest.y, min.y, max.y);

                auto circle2box = circle.pos - closest;

                return circle2box.lengthSqrt() <= circle.radiusSqrt();
            }
        }
        // Line Vs Line
        bool isColliding(Line2d& line1, Line2d& line2)
        {
            return false;
        }
        // Line Vs Circle
        bool isColliding(Line2d& line, Circle2d& circle)
        {
            return isColliding(circle, line);
        }
        // Line Vs Box
        bool isColliding(Line2d& line, Box2d& box)
        {
            return false;
        }
        /**********************************Box Collision*************************************/

        // Box Vs Box
        bool isColliding(Box2d& b1, Box2d& b2)
        {
            auto verts1 = b1.getVertices();
            auto verts2 = b2.getVertices();

            Points axices = {
                 (verts1[1] - verts1[0]).normal(),
                 (verts1[2] - verts1[1]).normal(),
                 (verts2[1] - verts2[0]).normal(),
                 (verts2[2] - verts2[1]).normal()
            };

            if (b1.rotation || b2.rotation && 0) {
                vec2f result;

                if (PUtils::TestVertices(verts1, verts2, axices)) return false;
                if (PUtils::TestVertices(verts2, verts1, axices)) return false;

                return true;
            }
            else {
                for (auto p : verts2) {
                    if (PUtils::PointInBox(p, b1)) return true;
                }

                for (auto p : verts1) {
                    if (PUtils::PointInBox(p, b2)) return true;
                }
                return false;
            }
        }
        // Box Vs Circle
        bool isColliding(Box2d& box, Circle2d& circle)
        {
            return false;
        }
        // Box Vs Line
        bool isColliding(Box2d& box, Line2d& line)
        {
            return false;
        }
    }
} // namespace Plutus
