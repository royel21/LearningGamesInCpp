#pragma once

namespace Plutus
{
    struct vec2f
    {
        float x = 0;
        float y = 0;
        vec2f() = default;
        vec2f(float val) : x(val), y(val) {}
        vec2f(float _x, float _y) : x(_x), y(_y) {}
        vec2f(int _x, int _y) : x((float)_x), y((float)_y) {}

        float length();
        float lengthSqrt();

        vec2f unit();
        vec2f normal();
        float dot(const vec2f& vec);
        float cross(const vec2f& vec);

        vec2f operator+(const vec2f& v) const;
        vec2f operator-(const vec2f& v) const;
        vec2f operator*(const vec2f& v) const;


        vec2f operator+(float) const;
        vec2f operator-(float) const;
        vec2f operator*(float v) const;
        vec2f operator/(float v) const;
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
        float z = 0;
        float w = 0;
        vec4f() = default;
        vec4f(float v) : x(v), y(v), z(v), w(v) {}
        vec4f(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    };
} // namespace Plutus