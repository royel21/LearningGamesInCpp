#include "Vectors.h"
#include "PMath.h"

namespace Plutus
{
    Vec2f::Vec2f(const Vec2i& v) : x((float)v.x), y((float)v.y) { }

    float Vec2f::lengthSqrt()
    {
        return x * x + y * y;
    }

    float Vec2f::length()
    {
        return 1 / invSqrt(x * x + y * y);
    }

    float Vec2f::invLength() {
        return invSqrt(x * x + y * y);
    }

    Vec2f Vec2f::unit()
    {
        float vecLength = length();
        if (vecLength > 0) {
            return { x / vecLength, y / vecLength };
        }
        else {
            return { 0,0 };
        }
    }

    Vec2f Vec2f::normal()
    {
        return Vec2f(-y, x);
    }

    float Vec2f::dot(const Vec2f& vec)
    {
        return x * vec.x + y * vec.y;
    }

    float Vec2f::cross(const Vec2f& vec)
    {
        return x * vec.y - y * vec.x;
    }

    Vec2f Vec2f::operator+(const Vec2f& v) const
    {
        return { x + v.x, y + v.y };
    }

    Vec2f Vec2f::operator-(const Vec2f& v) const
    {
        return { x - v.x, y - v.y };
    }

    Vec2f Vec2f::operator+(float val) const {
        return { x + val, y + val };
    }

    Vec2f Vec2f::operator-(float val) const {
        return { x - val, y - val };
    }

    Vec2f Vec2f::operator*(float v) const
    {
        return { x * v, y * v };
    }

    Vec2f Vec2f::operator/(float v) const
    {
        if (v == 0) return { x, y };
        return { x / v, y / v };
    }

    Vec2f Vec2f::operator-()
    {
        return { x * -1, y * -1 };
    }

    Vec2f& Vec2f::operator+=(const Vec2f& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vec2f& Vec2f::operator-=(const Vec2f& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vec2f& Vec2f::operator*=(float n)
    {
        x *= n;
        y *= n;
        return *this;
    }

    Vec2f& Vec2f::operator/=(float n)
    {
        x /= n;
        y /= n;
        return *this;
    }

    bool Vec2f::operator==(const Vec2f& v) const
    {
        return compareF(x, v.x, 0.00001f) && compareF(y, v.y, 0.00001f);
    }

    bool Vec2f::operator!=(const Vec2f& v) const
    {
        return !(*this == v);
    }

    bool Vec2f::operator< (const Vec2f& v) const {
        return x < v.x&& y < v.y;
    }
    bool Vec2f::operator> (const Vec2f& v) const {
        return x > v.x && y > v.y;
    }
    bool Vec2f::operator<= (const Vec2f& v) const {
        return x <= v.x && y <= v.y;
    }
    bool Vec2f::operator>= (const Vec2f& v) const {
        return x >= v.x && y >= v.y;
    }

    Vec2f Vec2f::getDirection(const Vec2f& v) {
        return (*this - v).unit();
    }
} // namespace Plutus
