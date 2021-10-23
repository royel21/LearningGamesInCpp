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

        vec2f operator+(float v) const;
        vec2f operator-(float v) const;
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

    struct vec2i {
        int x;
        int y;

        vec2i() : x(0), y(0) {};
        vec2i(int _x, int _y) : x(_x), y(_y) {};
        vec2i(float _x, float _y) : x((int)_x), y((int)_y) {};
    };

    struct vec3i {
        int x;
        int y;
        int z;

        vec3i() : x(0), y(0), z(0) {};
        vec3i(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {};
        vec3i(float _x, float _y, float _z) : x((int)_x), y((int)_y), z((int)_z) {};
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
        vec4f(float _x, float _y, int _z, int _w) : x(_x), y(_y), z((float)_z), w((float)_w) {}
        vec4f(int _x, int _y, int _z, int _w) : x((float)_x), y((float)_y), z((float)_z), w((float)_w) {}

        vec4f operator+(const vec4f& v) const;
        vec4f operator-(const vec4f& v) const;
    };
} // namespace Plutus