#include "Collider.h"
#include "PUtils.h"

#include <Math/PMath.h>
#include <Graphics/DebugRenderer.h>

#include "PUtils.h"
#include <algorithm>
#include <limits>
#include <Time/Timer.h>

namespace Plutus
{
    namespace Collider {

        //Circle Vs Circle
        bool isColliding(Circle2d* c1, Circle2d* c2, MTV* mtv)
        {
            float r = c1->radius + c2->radius;

            auto distVec = c1->pos - c2->pos;
            float sqrtLength = distVec.lengthSqrt();
            bool isColliding = sqrtLength < r* r;

            if (isColliding && mtv) {
                float length = 1 / invSqrt(sqrtLength);
                float collisionDepth = r - length;

                vec2f collisionDepthVec = (distVec / length) * collisionDepth;
                // collisionDepthVec *= 0.5f;
                c1->pos += collisionDepthVec;
                c2->pos -= collisionDepthVec;
            }
            return isColliding;
        }
        // Circle Vs Line
        bool isColliding(Circle2d* circle, Line2d* line, MTV* mtv)
        {
            auto closeVec = PUtils::ClosestPointLineCircle(circle, line);

            auto distVec = (circle->pos - closeVec);
            float lengthSqtr = distVec.lengthSqrt();
            bool result = lengthSqtr < circle->radiusSqrt();

            if (result && mtv) {
                float length = 1.0f / invSqrt(lengthSqtr);

                float sept = circle->radius - length;
                if (sept > 0) {
                    // circle->pos += (distVec / length) * sept;
                    mtv->axis += (distVec / length) * sept;
                }
            }
            // DebugRender::get()->drawCircle(closest, 5);
            // DebugRender::get()->drawLine(circle->pos, closest, 0);
            return result;
        }
        // Circle Vs Box
        bool isColliding(Circle2d* circle, Box2d* box, MTV* mtv)
        {
            auto min = box->pos;
            auto max = box->getMax();

            auto cpos = circle->pos;

            if (box->rotation)
                rotate(cpos, box->getCenter(), box->rotation);

            auto closest = cpos;

            closest.x = std::clamp(closest.x, min.x, max.x);
            closest.y = std::clamp(closest.y, min.y, max.y);

            auto dirVec = (cpos - closest);

            float lengthSqrt = dirVec.lengthSqrt();

            bool result = lengthSqrt < circle->radiusSqrt();

            if (result && mtv) {

                float length = 1 / invSqrt(lengthSqrt);
                // circle->pos += ((dirVec / length) * (circle->radius - length));
                mtv->axis += ((dirVec / length) * (circle->radius - length));
            }
            return result;
        }

        //Helpers

        bool getCollision(const Points& vertsA, const Points& vertsB, const Points& axesA, const Points& axesB, MTV* mtv) {
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

                if (overlap < mtv->dist) {
                    mtv->dist = overlap;
                    mtv->axis = axis;
                }
            }
            return false;
        }
        // Line Vs Line
        bool isColliding(Line2d* line1, Line2d* line2, MTV* mtv)
        {
            auto verts1 = line1->getVertices();
            auto verts2 = line2->getVertices();
            // MTV mtv;
            if (line2->axes[0] == line1->axes[0]) {
                return false;
            }
            else {
                if (PUtils::TestVertices(verts1, verts2)) return false;
                if (PUtils::TestVertices(verts2, verts1)) return false;
            }
            if (mtv) {

            }
            return true;
        }

        // Line Vs Circle
        bool isColliding(Line2d* line, Circle2d* circle, MTV* mtv)
        {
            return isColliding(circle, line, mtv);
        }

        // Line Vs Box
        bool isColliding(Line2d* line, Box2d* box, MTV* mtv)
        {
            mtv->dist = std::numeric_limits<float>::max();
            if (getCollision(line->getVertices(), box->getVertices(), line->axes, box->axes, mtv)) return false;

            if (mtv) {
                auto dir = line->getCenter() - box->getCenter();
                float dot = dir.dot(mtv->axis);
                if (dot < 0) {
                    mtv->axis = -mtv->axis;
                }
                box->pos -= (mtv->axis.unit() * mtv->dist);
            }
            return true;
        }
        /**********************************Box Collision*************************************/

        // Box Vs Box
        bool isColliding(Box2d* b1, Box2d* b2, MTV* mtv)
        {
            mtv->dist = std::numeric_limits<float>::max();
            if (getCollision(b1->getVertices(), b2->getVertices(), b1->axes, b2->axes, mtv)) return false;

            if (mtv) {
                auto dir = b2->getCenter() - b1->getCenter();
                if (dir.dot(mtv->axis) > 0) {
                    mtv->axis = -mtv->axis;
                }

                auto sept = (mtv->axis.unit() * mtv->dist) * 0.5f;
                b1->pos += sept;
                b2->pos -= sept;
            }
            return true;
        }
        // Box Vs Circle
        bool isColliding(Box2d* box, Circle2d* circle, MTV* mtv)
        {
            return isColliding(circle, box, mtv);
        }
        // Box Vs Line
        bool isColliding(Box2d* box, Line2d* line, MTV* mtv)
        {
            return isColliding(line, box, mtv);
        }
    }
} // namespace Plutus
