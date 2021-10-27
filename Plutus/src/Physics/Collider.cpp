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
                    DebugRender::get()->drawLine(circle->pos, closeVec, 0);
                    float length = sqrtf(lengthSqtr);
                    float sept = circle->radius - length;
                    if (sept > 0) {
                        circle->pos += (distVec / length) * sept;
                    }
                    // if (xDepth > 0 && yDepth > 0)
                    // {
                    //     if (std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f))
                    //     {
                    //         if (distVec.x < 0) {
                    //             circle->pos.x -= xDepth;
                    //         }
                    //         else {
                    //             circle->pos.x += xDepth;
                    //         }
                    //     }
                    //     else
                    //     {
                    //         if (distVec.y < 0) {
                    //             circle->pos.y -= yDepth;
                    //         }
                    //         else {
                    //             circle->pos.y += yDepth;
                    //         }
                    //     }
                    // }
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

            bool result = (circlePos - closest).lengthSqrt() <= circle->radiusSqrt();
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

            MTV mtv;
            mtv.dist = std::numeric_limits<float>::max();

            for (size_t i = 0; i < 3; i++)
            {
                bool firstAxes = i < line->axes.size();
                auto& axis = firstAxes ? line->axes[i] : box->axes[i - line->axes.size()];

                auto  proj1 = PUtils::getProjection(verts1, axis);
                auto  proj2 = PUtils::getProjection(verts2, axis);
                float overlap = std::min(proj2.y - proj1.x, proj1.y - proj2.x);
                if (overlap < 0) {
                    return false;
                }

                if (overlap < mtv.dist) {
                    mtv.dist = overlap;
                    mtv.axis = axis;
                    if (proj1.y > proj2.y) {
                        mtv.axis = -mtv.axis;
                    }
                }
            }

            if (manifold) {
                auto dir = line->getCenter() - box->getCenter();
                float dot = dir.dot(mtv.axis);
                if (dot < 0) {
                    mtv.axis = -mtv.axis;
                }
                box->pos += -(mtv.axis.unit() * mtv.dist);
            }
            return true;
        }
        /**********************************Box Collision*************************************/
        bool getCollision(const Points& vertsA, const Points& vertsB, const Points& axesA, const Points& axesB, MTV& mtv) {
            size_t acount = axesA.size();
            size_t count = acount + axesB.size();
            for (size_t i = 0; i < count; i++)
            {
                auto& axis = i < acount ? axesA[i] : axesB[i - acount];

                auto  proj1 = PUtils::getProjection(vertsA, axis);
                auto  proj2 = PUtils::getProjection(vertsB, axis);
                float overlap = std::min(proj2.y - proj1.x, proj1.y - proj2.x);
                if (overlap < 0) {
                    return true;
                }

                if (overlap < mtv.dist) {
                    mtv.dist = overlap;
                    mtv.axis = axis;
                }
            }
            return false;
        }
        // Box Vs Box
        bool isColliding(Box2d* b1, Box2d* b2, Manifold* manifold)
        {
            MTV mtv;
            mtv.dist = std::numeric_limits<float>::max();
            if (getCollision(b1->getVertices(), b2->getVertices(), b1->axes, b2->axes, mtv)) return false;

            if (manifold) {
                auto dir = b2->getCenter() - b1->getCenter();
                if (dir.dot(mtv.axis) > 0) {
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
