
#pragma once

#include <Math/Vectors.h>

#define float2u8(c) static_cast<uint8_t>(c * 255)
#define RGBA2Int(r, g, b, a) a << 24 | b << 16 | g << 8 | r

namespace Plutus
{
    struct ColorRGBA8
    {
        uint32_t rgba;
        //Empty Constructor
        ColorRGBA8() : rgba(0xffffffff) {}

        ColorRGBA8(uint32_t color) : rgba(color) {}

        ColorRGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) { rgba = RGBA2Int(r, g, b, a); }

        ColorRGBA8(const Vec4f& v) { setColor(v); }

        inline uint32_t get() { return rgba; };
        inline void setColor(uint32_t c) { rgba = c; }
        inline void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) { rgba = RGBA2Int(r, g, b, a); }

        inline void setColor(const Vec4f& v) {
            rgba = rgba = RGBA2Int(float2u8(v.x), float2u8(v.y), float2u8(v.z), float2u8(v.w));
        }

        operator uint32_t() const { return rgba; }
        //a << 24 | b << 16 | g << 8 | r
        operator Vec4f() const {
            return {
                (rgba & 0xff) / 255.0f,
                ((rgba >> 8) & 0xff) / 255.0f,
                ((rgba >> 16) & 0xff) / 255.0f,
                ((rgba >> 24) & 0xff) / 255.0f
            };
        }

        ColorRGBA8& operator=(const Vec4f& v) {
            setColor(v);
            return *this;
        }
    };
} // namespace Plutus