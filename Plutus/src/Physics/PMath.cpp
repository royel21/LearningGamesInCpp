#include "PMath.h"

#include <algorithm>

constexpr float PI = 3.14159265359f;
#define DEC2RA(dec) dec *(PI / 180.0f)

namespace Plutus
{
    void rotate(glm::vec2& point, const glm::vec2& origin, float angle)
    {
        float x = point.x - origin.x;
        float y = point.y - origin.y;

        float cosAng = cosf(DEC2RA(angle));
        float sinAng = sinf(DEC2RA(angle));

        float xPrime = x * cosAng - y * sinAng;
        float yPrime = x * sinAng + y * cosAng;

        point.x = xPrime + origin.x;
        point.y = yPrime + origin.y;
    }

    float dotProduct(const glm::vec2& vec1, const glm::vec2& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    bool compareF(float x, float y, float epsilon)
    {
        return abs(x - y) <= epsilon * std::max(1.0f, std::max(abs(x), abs(y)));
    }

    bool compare(const glm::vec2& v1, const glm::vec2& v2, float epsilon) {
        return compareF(v1.x, v2.x, epsilon) && compareF(v1.y, v2.y, epsilon);
    }
}