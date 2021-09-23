#include "IntersectDetector2d.h"
#include "PMath.h"

namespace Plutus
{
    bool IntersectDetector2d::pointOnLine(const glm::vec2& point, Line& line)
    {
        float dy = line.end.y - line.start.y;
        float dx = line.end.x - line.start.x;

        float m = dx == 0 ? 0 : dy / dx;



        float b = line.end.y - (m * line.end.x);

        //check the line equation
        return point.y == m * point.x + b;
    }

    bool IntersectDetector2d::pointInCircle(const glm::vec2& point, Circle& circle)
    {
        auto center = circle.getCenter();
        glm::vec2 centerToPoint = point - center;
        return centerToPoint.length() * centerToPoint.length() <= circle.radius * circle.radius;
    }

    bool IntersectDetector2d::pointAABB(const glm::vec2& point, AABB& box)
    {
        auto max = box.getMax();
        auto min = box.getMin();
        return point.x <= max.x && point.y <= max.y && point.x >= min.x && point.y >= min.y;
    }

    bool IntersectDetector2d::pointBox(const glm::vec2& point, Box2d& box)
    {
        glm::vec2 localPoint(point);
        auto max = box.getMax();
        auto min = box.getMin();

        rotate(localPoint, box.rBody->position, box.rBody->rotation);

        return localPoint.x <= max.x && localPoint.y <= max.y && localPoint.x >= min.x && localPoint.y >= min.y;
    }

    bool IntersectDetector2d::lineInCircle(Line line, Circle circle)
    {
        if (pointInCircle(line.start, circle) || pointInCircle(line.end, circle)) {
            return true;
        }

        auto lineSeg = line.end - line.start;
        auto circleCenter = circle.getCenter();

        auto centerToLineStart = circleCenter - line.start;
        float t = dotProduct(circleCenter, lineSeg) / dotProduct(lineSeg, lineSeg);
        //t is a percentage of the line and can't be smaller or bigger of the line size
        if (t < 0.0f || t > 1.0f) {
            return false;
        }

        //find the point close to the segment
        auto closestPoint = line.start + (lineSeg * t);

        return pointInCircle(closestPoint, circle);
    }


} // namespace Plutus
