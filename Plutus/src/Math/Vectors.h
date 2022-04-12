#pragma once

namespace Plutus
{
    struct vec2i;

    struct vec2f
    {
        float x = 0;
        float y = 0;
        vec2f() = default;
        vec2f(float val) : x(val), y(val) {}
        vec2f(float _x, float _y) : x(_x), y(_y) {}
        vec2f(int _x, int _y) : x((float)_x), y((float)_y) {}
        vec2f(const vec2i& v);

        inline void set(float _x, float _y) {
            x = _x;
            y = _y;
        }

        float length();
        float invLength();
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

        bool operator< (const vec2f& v) const;
        bool operator> (const vec2f& v) const;
        bool operator<= (const vec2f& v) const;
        bool operator>= (const vec2f& v) const;
    };

    struct vec3f {
        float x;
        float y;
        float z;

        vec3f() : x(0), y(0), z(0) {};
        vec3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
        vec3f(const vec2f& v, float _z = 0) : x(v.x), y(v.y), z(_z) {};
    };

    struct vec2i {
        int x;
        int y;

        vec2i() : x(0), y(0) {};
        vec2i(int _x, int _y) : x(_x), y(_y) {};
        vec2i(float _x, float _y) : x((int)_x), y((int)_y) {};
        vec2i(const vec2f& v);

        vec2i operator+(const vec2i& v) const;
        vec2i operator-(const vec2i& v) const;

        vec2i operator+(int v) const;
        vec2i operator-(int v) const;
        vec2i operator*(int v) const;
        vec2i operator/(int v) const;
        vec2i operator-();


        vec2i& operator*=(int n);
        vec2i& operator/=(int n);
        vec2i& operator+=(const vec2i& v);
        vec2i& operator-=(const vec2i& v);

        bool operator==(const vec2i& v) const;
        bool operator!=(const vec2i& v) const;

        bool operator< (const vec2i& v) const;
        bool operator> (const vec2i& v) const;
        bool operator<= (const vec2i& v) const;
        bool operator>= (const vec2i& v) const;
    };

    struct vec3i {
        int x;
        int y;
        int z;

        vec3i() : x(0), y(0), z(0) {};
        vec3i(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {};
        vec3i(float _x, float _y, float _z) : x((int)_x), y((int)_y), z((int)_z) {};
        vec3i(const vec3f& v) : x((int)v.x), y((int)v.y), z((int)v.z) {};
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

        vec4f(const vec2f& v, float _z = 0, float _w = 1) : x(v.x), y(v.y), z(_z), w(_w) {}
        vec4f(const vec2f& v1, const vec2f v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
        vec4f(const vec3f& v, float _w = 1) : x(v.x), y(v.y), z(v.z), w(_w) {}

        vec4f& operator*=(float n);
        vec4f& operator/=(float n);

        vec4f operator*(float v) const;
        vec4f operator/(float v) const;

        vec4f operator+(const vec4f& v) const;
        vec4f operator-(const vec4f& v) const;


    };
} // namespace Plutus