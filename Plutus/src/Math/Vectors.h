#pragma once

#include <array>

namespace Plutus
{
    struct Vec2i;

    struct Vec2f
    {
        float x = 0;
        float y = 0;
        Vec2f() = default;
        Vec2f(float val) : x(val), y(val) {}
        Vec2f(float _x, float _y) : x(_x), y(_y) {}
        Vec2f(int _x, int _y) : x((float)_x), y((float)_y) {}
        Vec2f(const Vec2i& v);

        inline void set(float _x, float _y) {
            x = _x;
            y = _y;
        }

        float length();
        float invLength();
        float lengthSqrt();

        Vec2f unit();
        Vec2f normal();
        float dot(const Vec2f& vec);
        float cross(const Vec2f& vec);

        Vec2f operator+(const Vec2f& v) const;
        Vec2f operator-(const Vec2f& v) const;

        Vec2f operator+(float v) const;
        Vec2f operator-(float v) const;
        Vec2f operator*(float v) const;
        Vec2f operator/(float v) const;
        Vec2f operator-();


        Vec2f& operator*=(float n);
        Vec2f& operator/=(float n);
        Vec2f& operator+=(const Vec2f& v);
        Vec2f& operator-=(const Vec2f& v);

        bool operator==(const Vec2f& v) const;
        bool operator!=(const Vec2f& v) const;

        bool operator< (const Vec2f& v) const;
        bool operator> (const Vec2f& v) const;
        bool operator<= (const Vec2f& v) const;
        bool operator>= (const Vec2f& v) const;

        Vec2f getDirection(const Vec2f& v);
    };

    struct Vec3f {
        float x;
        float y;
        float z;

        Vec3f() : x(0), y(0), z(0) {};
        Vec3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
        Vec3f(const Vec2f& v, float _z = 0) : x(v.x), y(v.y), z(_z) {};
    };

    struct Vec2i {
        int x;
        int y;

        Vec2i() : x(0), y(0) {};
        Vec2i(int _x, int _y) : x(_x), y(_y) {};
        Vec2i(float _x, float _y) : x((int)_x), y((int)_y) {};
        Vec2i(const Vec2f& v);

        Vec2i operator+(const Vec2i& v) const;
        Vec2i operator-(const Vec2i& v) const;

        Vec2i operator+(int v) const;
        Vec2i operator-(int v) const;
        Vec2i operator*(int v) const;
        Vec2i operator/(int v) const;
        Vec2i operator-();


        Vec2i& operator*=(int n);
        Vec2i& operator/=(int n);
        Vec2i& operator+=(const Vec2i& v);
        Vec2i& operator-=(const Vec2i& v);

        bool operator==(const Vec2i& v) const;
        bool operator!=(const Vec2i& v) const;

        bool operator< (const Vec2i& v) const;
        bool operator> (const Vec2i& v) const;
        bool operator<= (const Vec2i& v) const;
        bool operator>= (const Vec2i& v) const;
    };

    struct Vec3i {
        int x;
        int y;
        int z;

        Vec3i() : x(0), y(0), z(0) {};
        Vec3i(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {};
        Vec3i(float _x, float _y, float _z) : x((int)_x), y((int)_y), z((int)_z) {};
        Vec3i(const Vec3f& v) : x((int)v.x), y((int)v.y), z((int)v.z) {};
    };

    struct Vec4f
    {
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;
        Vec4f() = default;

        Vec4f(float v) : x(v), y(v), z(v), w(v) {}
        Vec4f(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
        Vec4f(float _x, float _y, int _z, int _w) : x(_x), y(_y), z((float)_z), w((float)_w) {}

        Vec4f(int _x, int _y, int _z, int _w) : x((float)_x), y((float)_y), z((float)_z), w((float)_w) {}

        Vec4f(const Vec2f& v, float _z = 0, float _w = 1) : x(v.x), y(v.y), z(_z), w(_w) {}
        Vec4f(const Vec2f& v1, const Vec2f v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) {}
        Vec4f(const Vec3f& v, float _w = 1) : x(v.x), y(v.y), z(v.z), w(_w) {}

        Vec4f& operator*=(float n);
        Vec4f& operator/=(float n);
        Vec4f operator-() const;

        Vec4f operator*(float v) const;
        Vec4f operator/(float v) const;

        Vec4f operator+(const Vec4f& v) const;
        Vec4f operator-(const Vec4f& v) const;

        bool contain(const Vec2f& v);
        bool contain(const Vec4f& v);
        bool overlap(const Vec4f& v);

        inline Vec2f getCenter() const { return { x + (z * 0.5f), y + (w * 0.5f) }; }
        inline Vec2f getLower() const { return { x, y }; }
        inline Vec2f getUpper() const { return { x + z, y + w }; }

        std::array<Vec2f, 4> getvertices() const {
            return std::array<Vec2f, 4>{ { {x, y}, { x,y + w }, { x + z, y + w }, { x + z, y } }};
        }
    };
} // namespace Plutus