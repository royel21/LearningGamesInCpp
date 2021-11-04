#include "PMath.h"

#include <algorithm>

constexpr float PI = 3.14159265359f;
#define DEC2RA(dec) dec *(PI / 180.0f)

namespace Plutus
{
    void rotate(vec2f& point, const vec2f& origin, float angle)
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

    bool compareF(float x, float y, float epsilon)
    {
        return abs(x - y) <= epsilon * std::max(1.0f, std::max(abs(x), abs(y)));
    }

    bool compare(const vec2f& v1, const vec2f& v2, float epsilon) {
        return compareF(v1.x, v2.x, epsilon) && compareF(v1.y, v2.y, epsilon);
    }
    //from  Quake III Arena Game and wikipedia
    float invSqrt(float n) {
        const float threehalfs = 1.5F;

        float x2 = n * 0.5F;
        float y = n;

        // evil floating point bit level hacking
        long i = *(long*)&y;

        // value is pre-assumed
        i = 0x5f3759df - (i >> 1);
        y = *(float*)&i;

        // 1st iteration
        y = y * (threehalfs - (x2 * y * y));
        y = y * (threehalfs - (x2 * y * y));
        y = y * (threehalfs - (x2 * y * y));
        return y;
    }
}