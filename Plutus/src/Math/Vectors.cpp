#include "Vectors.h"

#include "PMath.h"

namespace Plutus
{
    float vec2f::lengthSqrt()
    {
        return x * x + y * y;
    }

    float vec2f::length()
    {
        return 1 / invSqrt(x * x + y * y);
    }

    float vec2f::invLength() {
        return invSqrt(x * x + y * y);
    }

    vec2f vec2f::unit()
    {
        float vecLength = length();
        if (vecLength > 0) {
            return { x / vecLength, y / vecLength };
        }
        else {
            return { 0,0 };
        }
    }

    vec2f vec2f::normal()
    {
        return vec2f(-y, x);
    }

    float vec2f::dot(const vec2f& vec)
    {
        return x * vec.x + y * vec.y;
    }

    float vec2f::cross(const vec2f& vec)
    {
        return x * vec.y - y * vec.x;
    }

    vec2f vec2f::operator+(const vec2f& v) const
    {
        return { x + v.x, y + v.y };
    }

    vec2f vec2f::operator-(const vec2f& v) const
    {
        return { x - v.x, y - v.y };
    }

    vec2f vec2f::operator+(float val) const {
        return { x + val, y + val };
    }

    vec2f vec2f::operator-(float val) const {
        return { x - val, y - val };
    }

    vec2f vec2f::operator*(float v) const
    {
        return { x * v, y * v };
    }

    vec2f vec2f::operator/(float v) const
    {
        if (v == 0) return { x, y };
        return { x / v, y / v };
    }

    vec2f vec2f::operator-()
    {
        return { x * -1, y * -1 };
    }

    vec2f& vec2f::operator+=(const vec2f& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    vec2f& vec2f::operator-=(const vec2f& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    vec2f& vec2f::operator*=(float n)
    {
        x *= n;
        y *= n;
        return *this;
    }

    vec2f& vec2f::operator/=(float n)
    {
        x /= n;
        y /= n;
        return *this;
    }

    bool vec2f::operator==(const vec2f& v) const
    {
        return compareF(x, v.x, 0.00001f) && compareF(y, v.y, 0.00001f);
    }

    bool vec2f::operator!=(const vec2f& v) const
    {
        return !(*this == v);
    }

    bool vec2f::operator< (const vec2f& v) const {
        return x < v.x&& y < v.y;
    }
    bool vec2f::operator> (const vec2f& v) const {
        return x > v.x && y > v.y;
    }
    bool vec2f::operator<= (const vec2f& v) const {
        return x <= v.x && y <= v.y;
    }
    bool vec2f::operator>= (const vec2f& v) const {
        return x >= v.x && y >= v.y;
    }

    /***********************************************************************/

    vec4f& vec4f::operator*=(float n) {
        x *= n;
        y *= n;
        z *= n;
        w *= n;

        return *this;
    }

    vec4f& vec4f::operator/=(float n) {
        x /= n;
        y /= n;
        z /= n;
        w /= n;

        return *this;
    }

    vec4f vec4f::operator*(float n) const {
        return { x * n, y * n, z * n, w * n };
    }

    vec4f vec4f::operator/(float n) const {
        return { x / n, y / n, z / n, w / n };
    }

    vec4f vec4f::operator+(const vec4f& v) const
    {
        return { x + v.x, y + v.y, z + v.z, w + v.w };
    }

    vec4f vec4f::operator-(const vec4f& v) const
    {
        return { x - v.x, y - v.y, z - v.z, w - v.w };
    }
} // namespace Plutus
