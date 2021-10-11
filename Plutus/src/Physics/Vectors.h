#pragma once

namespace Plutus
{
    struct vec2f
    {
        float x = 0;
        float y = 0;
        vec2f() = default;
        vec2f(float _x, float _y) : x(_x), y(_y) {}

        float length();
        float lengthSqrt();

        vec2f unit();
        vec2f normal();
        float dot(const vec2f& vec);
        float cross(const vec2f& vec);

        vec2f operator+(const vec2f& v);
        vec2f operator-(const vec2f& v) const;
        vec2f operator*(float v) const;
        vec2f operator/(float v);
        vec2f operator-();

        vec2f& operator*=(float n);
        vec2f& operator/=(float n);
        vec2f& operator+=(const vec2f& v);
        vec2f& operator-=(const vec2f& v);

        bool operator==(const vec2f& v) const;
        bool operator!=(const vec2f& v) const;
    };

    struct vec4f
    {
        float x = 0;
        float y = 0;

        vec4f(float _x, float _y) : x(_x), y(_y) {}

        float lengthSqrt();
        float length();

        float unit() const;
        float dot(const vec4f& vec);
    };

} // namespace Plutus