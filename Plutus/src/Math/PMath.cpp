#include "PMath.h"
#include <math.h>

#include <algorithm>

namespace Plutus
{
    void rotate(Vec2f& point, const Vec2f& origin, float angle)
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

    Vec2f rotateP(const Vec2f& point, const Vec2f& origin, float angle)
    {
        float x = point.x - origin.x;
        float y = point.y - origin.y;

        float cosAng = cosf(DEC2RA(angle));
        float sinAng = sinf(DEC2RA(angle));

        float xPrime = x * cosAng - y * sinAng;
        float yPrime = x * sinAng + y * cosAng;

        return { xPrime + origin.x,yPrime + origin.y };
    }

    bool compareF(float x, float y, float epsilon)
    {
        return abs(x - y) <= epsilon * std::max(1.0f, std::max(abs(x), abs(y)));
    }

    bool compare(const Vec2f& v1, const Vec2f& v2, float epsilon) {
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

    Vec2f max(const Vec2f& a, float y) {
        return { std::max(a.x, y), std::max(a.y, y) };
    }
}