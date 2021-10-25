#include "Collider.h"

#include <Math/PMath.h>

#define CLAMPVAL(v, min, max) v < min ? min : v > max ? max : v

namespace Plutus
{
    bool Collider::pointOnLine(vec2f& point, Line2d& line)
    {
        float dy = line.end.y - line.pos.y;
        float dx = line.end.x - line.pos.x;
        if (dx == 0) {
            return compareF(point.x, line.pos.x);
        }

        float m = dy / dx;
        float b = line.end.y - (m * line.end.x);

        //check the line equation
        return point.y == m * point.x + b;
    }

    bool Collider::pointBox(vec2f& point, Box2d& box)
    {
        vec2f p = point;
        if (box.rotation) {
            rotate(p, box.getCenter(), box.rotation);
        }
        return p >= box.pos && p <= box.getMax();
    }


    bool Collider::pointInCircle(vec2f& p, Circle2d& c)
    {
        return (p - c.pos).lengthSqrt() - 1 < c.radiusSqrt();
    }

    //Circle Vs Circle
    bool Collider::isColliding(Circle2d& c1, Circle2d& c2)
    {
        float r = c1.radius + c2.radius;
        return (c2.pos - c1.pos).lengthSqrt() - 1 <= r * r;
    }
    // Circle Vs Line
    bool Collider::isColliding(Circle2d& circle, Line2d& line)
    {
        if (pointInCircle(line.pos, circle) || pointInCircle(line.end, circle)) {
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

        return pointInCircle(closestPoint, circle);
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
    bool Collider::isColliding(Circle2d& circle, Box2d& box)
    {
        if (box.rotation) {
            return rotateBox(circle, box);
        }
        else {
            auto max = box.getMax();
            auto min = box.pos;
            auto closest = circle.pos;

            closest.x = CLAMPVAL(closest.x, min.x, max.x);
            closest.y = CLAMPVAL(closest.y, min.y, max.y);

            auto circle2box = circle.pos - closest;

            return circle2box.lengthSqrt() <= circle.radiusSqrt();
        }
    }
    // Line Vs Line
    bool Collider::isColliding(Line2d& line1, Line2d& line2)
    {
        return false;
    }
    // Line Vs Circle
    bool Collider::isColliding(Line2d& line, Circle2d& circle)
    {
        return isColliding(circle, line);
    }
    // Line Vs Box
    bool Collider::isColliding(Line2d& line, Box2d& box)
    {
        return false;
    }
    /**********************************Box Collision*************************************/

    // Box Vs Circle
    bool Collider::isColliding(Box2d& box, Circle2d& circle)
    {
        return false;
    }
    // Box Vs Line
    bool Collider::isColliding(Box2d& box, Line2d& line)
    {
        return false;
    }



} // namespace Plutus
