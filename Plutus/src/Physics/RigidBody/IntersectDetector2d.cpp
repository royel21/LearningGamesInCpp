#include "IntersectDetector2d.h"

namespace Plutus
{
    bool IntersectDetector2d::pointOnLine(const glm::vec2& point, Line line)
    {
        float dy = line.end.y - line.start.y;
        float dx = line.end.x - line.start.x;
        float m = dy / dx;

        float b = line.end.y - (m * line.end.x);

        //check the line equation
        return point.y == m * point.x + b;
    }

    bool IntersectDetector2d::pointInCircle(glm::vec2 point, Circle circle)
    {
        auto center = circle.getCenter();
        glm::vec2 centerToPoint = point - center;
        return centerToPoint.length() * centerToPoint.length() <= circle.radius * circle.radius;
    }


} // namespace Plutus
