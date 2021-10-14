#include "Collider.h"
#include "PMath.h"

namespace Plutus
{
    bool Collider::pointOnLine(const vec2f& point, Line2d& line)
    {
        float dy = line.end.y - line.start.y;
        float dx = line.end.x - line.start.x;
        if (dx == 0) {
            return compareF(point.x, line.start.x);
        }

        float m = dy / dx;
        float b = line.end.y - (m * line.end.x);

        //check the line equation
        return point.y == m * point.x + b;
    }

    bool Collider::pointInCircle(const vec2f& point, Circle2d& circle)
    {
        // auto center = circle.getCenter();
        // Vector2d centerToPoint = point - center;
        // return centerToPoint.length() * centerToPoint.length() <= circle.radius * circle.radius;
        return false;
    }

    // bool Collider::pointAABB(const Vector2d& point, AABB& box)
    // {
    //     // auto max = box.getMax();
    //     // auto min = box.getMin();
    //     // return point.x <= max.x && point.y <= max.y && point.x >= min.x && point.y >= min.y;
    // }

    bool Collider::pointBox(const vec2f& point, Box2d& box)
    {
        // Vector2d localPoint(point);
        // auto max = box.getMax();
        // auto min = box.getMin();

        // rotate(localPoint, box.rBody->position, box.rBody->rotation);

        // return localPoint.x <= max.x && localPoint.y <= max.y && localPoint.x >= min.x && localPoint.y >= min.y;
        return false;
    }

    bool Collider::lineInCircle(Line2d line, Circle2d circle)
    {
        // if (pointInCircle(line.start, circle) || pointInCircle(line.end, circle)) {
        //     return true;
        // }

        // auto lineSeg = line.end - line.start;
        // auto circleCenter = circle.getCenter();

        // auto centerToLineStart = circleCenter - line.start;
        // float t = dotProduct(circleCenter, lineSeg) / dotProduct(lineSeg, lineSeg);
        // //t is a percentage of the line and can't be smaller or bigger of the line size
        // if (t < 0.0f || t > 1.0f) {
        //     return false;
        // }

        // //find the point close to the segment
        // auto closestPoint = line.start + (lineSeg * t);

        // return pointInCircle(closestPoint, circle);
        return false;
    }


} // namespace Plutus
