
#pragma once

#include <Math/Vectors.h>

#define float2u8(c) static_cast<uint8_t>(c * 255)
#define RGBA2Int(r, g, b, a) a << 24 | b << 16 | g << 8 | r

#define ALPHA 0xffffff

namespace Plutus
{
    struct ColorRGBA8
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
        ColorRGBA8(uint32_t color = 0xffffffff) { setColor(color); }

        ColorRGBA8(uint8_t _r, uint8_t _g = 255, uint8_t _b = 255, uint8_t _a = 255) : r(_r), g(_g), b(_b), a(_a) {}

        ColorRGBA8(const Vec4f& v) { setColor(v); }

        inline uint32_t get() { return r | g << 8 | b << 16 | a << 24; };

        inline void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
            this->r = r; this->g = g; this->b = b; this->a = a;
        }

        inline void setColor(uint32_t c) {
            r = c & 255;
            g = (c >> 8) & 255;
            b = (c >> 16) & 255;
            a = (c >> 24) & 255;
        }

        inline void setColor(const Vec4f& v) {
            r = uint8_t(v.x * 255);
            g = uint8_t(v.y * 255);
            b = uint8_t(v.z * 255);
            a = uint8_t(v.w * 255);
        }

        operator uint32_t() const { return r | g << 8 | b << 16 | a << 24;; }
        //a << 24 | b << 16 | g << 8 | r
        operator Vec4f() const {
            return {
                r / 255.0f,
                g / 255.0f,
                b / 255.0f,
                a / 255.0f
            };
        }

        ColorRGBA8& operator=(int value) {
            setColor(value);
            return *this;
        }

        ColorRGBA8& operator=(const Vec4f& v) {
            setColor(v);
            return *this;
        }
    };
} // namespace Plutus