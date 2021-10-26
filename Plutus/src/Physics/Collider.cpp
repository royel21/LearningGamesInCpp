#include "Collider.h"
#include "PUtils.h"

#include <Math/PMath.h>
#include <Graphics/DebugRenderer.h>

#define CLAMPVAL(v, min, max) v < min ? min : v > max ? max : v

namespace Plutus
{
    namespace Collider {

        //Circle Vs Circle
        bool isColliding(Circle2d* c1, Circle2d* c2, Manifold* manifold)
        {

            float r = c1->radius + c2->radius;
            auto dist1 = c1->pos - c2->pos;
            float sqrtLength = dist1.lengthSqrt();
            bool isColliding = sqrtLength <= r * r;

            if (isColliding && manifold) {
                float length = sqrtf(sqrtLength);
                float pen_deth = r - length;
                manifold->forces.push_back((dist1 / length) * pen_deth);
            }
            return isColliding;
        }
        // Circle Vs Line
        bool isColliding(Circle2d* circle, Line2d* line, Manifold* manifold)
        {
            auto closeVec = PUtils::ClosestPointLineCircle(circle, line);

            bool result = false;

            auto distVec = (circle->pos - closeVec);
            float lengthSqtr = distVec.lengthSqrt();

            if (lengthSqtr <= circle->radiusSqrt()) {
                if (manifold) {
                    float length = sqrt(lengthSqtr);
                    manifold->forces.push_back((distVec / length) * (circle->radius - length));
                }
                result = true;
            }
            return result;
        }
        // Circle Vs Box
        bool isColliding(Circle2d* circle, Box2d* box, Manifold* manifold)
        {
            auto min = box->pos;
            auto max = box->getMax();

            auto circlePos = circle->pos;

            if (box->rotation)
                rotate(circlePos, box->getCenter(), box->rotation);

            auto closest = circlePos;

            closest.x = CLAMPVAL(closest.x, min.x, max.x);
            closest.y = CLAMPVAL(closest.y, min.y, max.y);

            auto circle2box = circlePos - closest;
            bool result = circle2box.lengthSqrt() <= circle->radiusSqrt();
            if (result && manifold) {

            }
            return result;
        }
        // Line Vs Line
        bool isColliding(Line2d* line1, Line2d* line2, Manifold* manifold)
        {
            auto verts1 = line1->getVertices();
            auto verts2 = line2->getVertices();
            if (PUtils::TestVertices(verts1, verts2)) return false;
            if (PUtils::TestVertices(verts2, verts1)) return false;
            if (manifold) {

            }
            return true;
        }
        // Line Vs Circle
        bool isColliding(Line2d* line, Circle2d* circle, Manifold* manifold)
        {
            return isColliding(circle, line, manifold);
        }
        // Line Vs Box
        bool isColliding(Line2d* line, Box2d* box, Manifold* manifold)
        {
            auto verts1 = line->getVertices();
            auto verts2 = box->getVertices();

            Points axes = {
                 (verts1[1] - verts1[0]).normal(),
                 (verts2[1] - verts2[0]).normal(),
                 (verts2[2] - verts2[1]).normal()
            };

            if (PUtils::TestVertices(verts1, verts2, axes)) return false;
            if (PUtils::TestVertices(verts2, verts1, axes)) return false;

            if (manifold) {

            }
            return true;
        }
        /**********************************Box Collision*************************************/

        // Box Vs Box
        bool isColliding(Box2d* b1, Box2d* b2, Manifold* manifold)
        {
            auto verts1 = b1->getVertices();
            auto verts2 = b2->getVertices();
            bool result = true;

            if (b1->rotation || b2->rotation) {
                vec2f result;

                Points axes = {
                     (verts1[1] - verts1[0]).normal(),
                     (verts1[2] - verts1[1]).normal(),
                     (verts2[1] - verts2[0]).normal(),
                     (verts2[2] - verts2[1]).normal()
                };

                if (PUtils::TestVertices(verts1, verts2, axes)) return false;
                if (PUtils::TestVertices(verts2, verts1, axes)) return false;
                if (manifold) {

                }
                return true;
            }
            else {

                for (auto p : verts1) {
                    if (PUtils::PointInBox(p, b2)) {
                        if (manifold) {

                        }
                        return true;
                    }
                }
                return false;
            }
        }
        // Box Vs Circle
        bool isColliding(Box2d* box, Circle2d* circle, Manifold* manifold)
        {
            return isColliding(circle, box, manifold);
        }
        // Box Vs Line
        bool isColliding(Box2d* box, Line2d* line, Manifold* manifold)
        {
            return isColliding(line, box, manifold);
        }
    }
} // namespace Plutus
