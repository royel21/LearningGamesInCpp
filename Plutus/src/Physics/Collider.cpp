#include "Collider.h"
#include "PUtils.h"

#include <Math/PMath.h>
#include <Graphics/DebugRenderer.h>

#include "PUtils.h"
#include <algorithm>
#include <limits>

#define CLAMPVAL(v, min, max) v < min ? min : v > max ? max : v

namespace Plutus
{
    namespace Collider {
        //Circle Vs Circle
        bool isColliding(Circle2d* c1, Circle2d* c2, Manifold* manifold)
        {

            float r = c1->radius + c2->radius;

            auto distVec = c1->pos - c2->pos;
            float sqrtLength = distVec.lengthSqrt();
            bool isColliding = sqrtLength <= r * r;

            if (isColliding && manifold) {
                float length = sqrtf(sqrtLength);
                float collisionDepth = r - length;

                vec2f collisionDepthVec = (distVec / length) * collisionDepth;
                // DebugRender::get()->drawLine(c1->pos, collisionDepthVec, 0);
                collisionDepth *= 0.5f;
                c1->pos += collisionDepthVec;
                c2->pos -= collisionDepthVec;
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
                    // float length = circle->radius - sqrt(lengthSqtr);
                    // float depth = circle->radius - length;
                    // vec2f force = circle->pos + (distVec / length) * depth;
                    // manifold->forces.push_back((distVec / length) * (circle->radius - length));
                    DebugRender::get()->drawLine(circle->pos, closeVec, 0);
                    float xDepth = circle->radius - abs(distVec.x);
                    float yDepth = circle->radius - abs(distVec.y);

                    if (xDepth > 0 && yDepth > 0)
                    {
                        if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
                        {
                            if (distVec.x < 0) {
                                circle->pos.x -= xDepth;
                            }
                            else {
                                circle->pos.x += xDepth;
                            }
                        }
                        else
                        {
                            if (distVec.y < 0) {
                                circle->pos.y -= yDepth;
                            }
                            else {
                                circle->pos.y += yDepth;
                            }
                        }
                    }
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
            MTV mtv;
            mtv.dist = std::numeric_limits<float>::max();

            auto verts1 = b1->getVertices();
            auto verts2 = b2->getVertices();
            bool result = true;

            Points axes = {
                 (verts1[1] - verts1[0]).normal(),
                 (verts1[2] - verts1[1]).normal(),
                 (verts2[1] - verts2[0]).normal(),
                 (verts2[2] - verts2[1]).normal()
            };

            for (auto& axis : axes) {
                auto  proj1 = PUtils::getProjection(verts1, axis);
                auto  proj2 = PUtils::getProjection(verts2, axis);
                if (proj1.x >= proj2.y || proj2.x >= proj1.y) {
                    return false;
                }
                else {
                    float ov = std::min(proj2.y - proj1.x, proj1.y - proj2.x);
                    if (ov < mtv.dist) {
                        mtv.dist = ov;
                        mtv.axis = axis;
                    }
                }
            }

            for (auto& axis : axes) {
                auto  proj1 = PUtils::getProjection(verts1, axis);
                auto  proj2 = PUtils::getProjection(verts2, axis);
                if (proj1.x >= proj2.y || proj2.x >= proj1.y) {
                    return false;
                }
                else {
                    // float axisDepth = MathF.Min(maxB - minA, maxA - minB);
                    float ov = std::min(proj2.y - proj1.x, proj1.y - proj2.x);
                    float ov2 = std::min(proj1.y, proj2.y) - std::max(proj1.x, proj2.x);
                    if (ov < mtv.dist) {
                        mtv.dist = ov = ov2;
                        mtv.axis = axis;
                    }
                }
            }
            if (manifold) {
                auto dir = b2->getCenter() - b1->getCenter();
                if (dir.dot(mtv.axis)) {
                    mtv.axis = -mtv.axis;
                }
                auto sept = (mtv.axis.unit() * mtv.dist) * 0.5f;
                b1->pos += sept;
                b2->pos -= sept;
            }
            return true;
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
