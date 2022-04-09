#include "Vectors.h"

#include "PMath.h"

namespace Plutus
{
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
