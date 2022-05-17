#include "Vectors.h"

#include "PMath.h"

namespace Plutus
{
    Vec4f& Vec4f::operator*=(float n) {
        x *= n;
        y *= n;
        z *= n;
        w *= n;

        return *this;
    }

    Vec4f& Vec4f::operator/=(float n) {
        x /= n;
        y /= n;
        z /= n;
        w /= n;

        return *this;
    }
    Vec4f Vec4f::operator-() const {
        return { -x, -y, -z, -w };
    }

    Vec4f Vec4f::operator*(float n) const {
        return { x * n, y * n, z * n, w * n };
    }

    Vec4f Vec4f::operator/(float n) const {
        return { x / n, y / n, z / n, w / n };
    }

    Vec4f Vec4f::operator+(const Vec4f& v) const
    {
        return { x + v.x, y + v.y, z + v.z, w + v.w };
    }

    Vec4f Vec4f::operator-(const Vec4f& v) const
    {
        return { x - v.x, y - v.y, z - v.z, w - v.w };
    }

    bool Vec4f::contain(const Vec2f& v) {
        return !(v.x < x || v.y < y || v.x >= (x + z) || v.y >= (y + w));
    }

    bool Vec4f::contain(const Vec4f& v) {
        return (v.x >= x && (v.x + v.z) < (x + z) && (v.y > y) && (v.y + v.w) < (y + w));
    }

    bool Vec4f::overlap(const Vec4f& v) {
        // return v.x + v.z >= x || v.y + v.w >= y || v.x <= (x + z) || y <= (y + w);
        // return (x < v.x + v.z && x + z >= v.x && y < v.y + v.w && y + w >= v.y);
        return
            contain(Vec2f{ v.x, v.y }) ||
            contain(Vec2f{ v.x, v.w }) ||
            contain(Vec2f{ v.z, v.w }) ||
            contain(Vec2f{ v.x, v.w });
    }
} // namespace Plutus
