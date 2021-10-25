#include "Collider.h"
#include "PUtils.h"

#include <Math/PMath.h>
#include <Graphics/DebugRenderer.h>

#define CLAMPVAL(v, min, max) v < min ? min : v > max ? max : v

namespace Plutus
{
    namespace Collider {

        //Circle Vs Circle
        bool isColliding(Circle2d& c1, Circle2d& c2, bool response)
        {

            float r = c1.radius + c2.radius;
            auto dist1 = c1.pos - c2.pos;
            float sqrtLength = dist1.lengthSqrt();
            bool isColliding = sqrtLength <= r * r;

            if (isColliding && response) {
                float length = sqrtf(sqrtLength);
                float pen_deth = r - length;
                c1.pos += (dist1 / length) * pen_deth;
            }
            return isColliding;
        }
        // Circle Vs Line
        bool isColliding(Circle2d& circle, Line2d& line, bool response)
        {
            auto vertices = line.getVertices();
            auto p1 = vertices[0];
            auto p2 = vertices[1];

            bool result = false;
            vec2f closestPoint;
            vec2f dist = circle.pos - p1;
            float csqrt = circle.radiusSqrt();

            float lengthSqtr = dist.lengthSqrt();

            if (lengthSqtr - 1 < csqrt) {
                result = true;
                closestPoint = p1;
            }
            if (!result) {
                dist = circle.pos - p2;
                lengthSqtr = dist.lengthSqrt();

                result = lengthSqtr - 1 < csqrt;
                closestPoint = p2;
            }

            if (!result)
            {
                auto lineSeg = p2 - p1;

                auto centerToLineStart = circle.pos - p1;

                float t = centerToLineStart.dot(lineSeg) / lineSeg.dot(lineSeg);
                // t is a percentage of the line and can't be smaller or bigger of the line size
                if (t < 0.0f || t > 1.0f) {
                    return false;
                }

                //find the point close to the segment
                closestPoint = p1 + (lineSeg * t);

                dist = circle.pos - closestPoint;
                lengthSqtr = dist.lengthSqrt();

                result = lengthSqtr - 1 < csqrt;
            }

            if (result && response) {

                auto length = sqrt(lengthSqtr);

                auto pos = (dist / length) * (circle.radius - length);
                pos = 0;
            }
            DebugRender::get()->drawLine(circle.pos, closestPoint, 0);
            return result;
        }
        // Circle Vs Box
        bool isColliding(Circle2d& circle, Box2d& box, bool response)
        {
            auto min = box.pos;
            auto max = box.getMax();

            auto circlePos = circle.pos;

            if (box.rotation)
                rotate(circlePos, box.getCenter(), box.rotation);

            auto closest = circlePos;

            closest.x = CLAMPVAL(closest.x, min.x, max.x);
            closest.y = CLAMPVAL(closest.y, min.y, max.y);

            auto circle2box = circlePos - closest;
            bool result = circle2box.lengthSqrt() <= circle.radiusSqrt();
            if (result && response) {

            }
            return result;
        }
        // Line Vs Line
        bool isColliding(Line2d& line1, Line2d& line2, bool response)
        {
            auto verts1 = line1.getVertices();
            auto verts2 = line2.getVertices();
            if (PUtils::TestVertices(verts1, verts2)) return false;
            if (PUtils::TestVertices(verts2, verts1)) return false;
            if (response) {

            }
            return true;
        }
        // Line Vs Circle
        bool isColliding(Line2d& line, Circle2d& circle, bool response)
        {
            return isColliding(circle, line, response);
        }
        // Line Vs Box
        bool isColliding(Line2d& line, Box2d& box, bool response)
        {
            auto verts1 = line.getVertices();
            auto verts2 = box.getVertices();

            Points axes = {
                 (verts1[1] - verts1[0]).normal(),
                 (verts2[1] - verts2[0]).normal(),
                 (verts2[2] - verts2[1]).normal()
            };

            if (PUtils::TestVertices(verts1, verts2, axes)) return false;
            if (PUtils::TestVertices(verts2, verts1, axes)) return false;

            if (response) {

            }
            return true;
        }
        /**********************************Box Collision*************************************/

        // Box Vs Box
        bool isColliding(Box2d& b1, Box2d& b2, bool response)
        {
            auto verts1 = b1.getVertices();
            auto verts2 = b2.getVertices();
            bool result = true;

            if (b1.rotation || b2.rotation && 0) {
                vec2f result;

                Points axes = {
                     (verts1[1] - verts1[0]).normal(),
                     (verts1[2] - verts1[1]).normal(),
                     (verts2[1] - verts2[0]).normal(),
                     (verts2[2] - verts2[1]).normal()
                };

                if (PUtils::TestVertices(verts1, verts2, axes)) return false;
                if (PUtils::TestVertices(verts2, verts1, axes)) return false;
                if (response) {

                }
                return true;
            }
            else {

                for (auto p : verts1) {
                    if (PUtils::PointInBox(p, b2)) {
                        if (response) {

                        }
                        return true;
                    }
                }
                return false;
            }
        }
        // Box Vs Circle
        bool isColliding(Box2d& box, Circle2d& circle, bool response)
        {
            return isColliding(circle, box, response);
        }
        // Box Vs Line
        bool isColliding(Box2d& box, Line2d& line, bool response)
        {
            return isColliding(line, box, response);
        }
    }
} // namespace Plutus
