#include "Vectors.h"
#include "PMath.h"

namespace Plutus
{
    vec2i::vec2i(const vec2f& v) : x((int)v.x), y((int)v.y) { }


    vec2i vec2i::operator+(const vec2i& v) const
    {
        return { x + v.x, y + v.y };
    }

    vec2i vec2i::operator-(const vec2i& v) const
    {
        return { x - v.x, y - v.y };
    }

    vec2i vec2i::operator+(int val) const {
        return { x + val, y + val };
    }

    vec2i vec2i::operator-(int val) const {
        return { x - val, y - val };
    }

    vec2i vec2i::operator*(int v) const
    {
        return { x * v, y * v };
    }

    vec2i vec2i::operator/(int v) const
    {
        if (v == 0) return { x, y };
        return { x / v, y / v };
    }

    vec2i vec2i::operator-()
    {
        return { x * -1, y * -1 };
    }

    vec2i& vec2i::operator+=(const vec2i& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    vec2i& vec2i::operator-=(const vec2i& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    vec2i& vec2i::operator*=(int n)
    {
        x *= n;
        y *= n;
        return *this;
    }

    vec2i& vec2i::operator/=(int n)
    {
        x /= n;
        y /= n;
        return *this;
    }

    bool vec2i::operator==(const vec2i& v) const
    {
        return x == v.x && y == v.y;
    }

    bool vec2i::operator!=(const vec2i& v) const
    {
        return !(*this == v);
    }

    bool vec2i::operator< (const vec2i& v) const {
        return x < v.x&& y < v.y;
    }
    bool vec2i::operator> (const vec2i& v) const {
        return x > v.x && y > v.y;
    }
    bool vec2i::operator<= (const vec2i& v) const {
        return x <= v.x && y <= v.y;
    }
    bool vec2i::operator>= (const vec2i& v) const {
        return x >= v.x && y >= v.y;
    }
} // namespace Plutus
