#include "IntersectDetector2d.h"

namespace Plutus
{
    bool IntersectDetector2d::pointOnLine(const glm::vec2& point, const Line& line)
    {
        float dy = line.end.y - line.start.y;
        float dx = line.end.x - line.start.x;
        float m = dy / dx;

        float b = line.end.y - (m * line.end.x);

        //check the line equation
        return point.y == m * point.x + b;
    }

    bool IntersectDetector2d::pointInCircle(const glm::vec2& point, const Circle& circle)
    {
        auto center = circle.getCenter();
        glm::vec2 centerToPoint = point - center;
        return centerToPoint.length() * centerToPoint.length() <= circle.radius * circle.radius;
    }

    bool IntersectDetector2d::pointAABB(const glm::vec2& point, const AABB& box)
    {
        auto max = box.getMax();
        auto min = box.getMin();
        return point.x <= max.x && point.y <= max.y && point.x >= min.x && point.y >= min.y;
    }


} // namespace Plutus
