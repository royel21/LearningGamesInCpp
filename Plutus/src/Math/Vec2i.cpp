#include "Vectors.h"
#include "PMath.h"

namespace Plutus
{
    Vec2i::Vec2i(const Vec2f& v) : x((int)v.x), y((int)v.y) { }


    Vec2i Vec2i::operator+(const Vec2i& v) const
    {
        return { x + v.x, y + v.y };
    }

    Vec2i Vec2i::operator-(const Vec2i& v) const
    {
        return { x - v.x, y - v.y };
    }

    Vec2i Vec2i::operator+(int val) const {
        return { x + val, y + val };
    }

    Vec2i Vec2i::operator-(int val) const {
        return { x - val, y - val };
    }

    Vec2i Vec2i::operator*(int v) const
    {
        return { x * v, y * v };
    }

    Vec2i Vec2i::operator/(int v) const
    {
        if (v == 0) return { x, y };
        return { x / v, y / v };
    }

    Vec2i Vec2i::operator-()
    {
        return { x * -1, y * -1 };
    }

    Vec2i& Vec2i::operator+=(const Vec2i& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vec2i& Vec2i::operator-=(const Vec2i& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vec2i& Vec2i::operator*=(int n)
    {
        x *= n;
        y *= n;
        return *this;
    }

    Vec2i& Vec2i::operator/=(int n)
    {
        x /= n;
        y /= n;
        return *this;
    }

    bool Vec2i::operator==(const Vec2i& v) const
    {
        return x == v.x && y == v.y;
    }

    bool Vec2i::operator!=(const Vec2i& v) const
    {
        return !(*this == v);
    }

    bool Vec2i::operator< (const Vec2i& v) const {
        return x < v.x&& y < v.y;
    }
    bool Vec2i::operator> (const Vec2i& v) const {
        return x > v.x && y > v.y;
    }
    bool Vec2i::operator<= (const Vec2i& v) const {
        return x <= v.x && y <= v.y;
    }
    bool Vec2i::operator>= (const Vec2i& v) const {
        return x >= v.x && y >= v.y;
    }
} // namespace Plutus
