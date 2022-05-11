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
} // namespace Plutus
