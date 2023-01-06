#pragma once
#include <Math/Vectors.h>
#include <array>

namespace Plutus
{
    struct TransformComponent
    {
        float x;
        float y;
        float offsetX = 0;
        float offsetY = 0;
        int w;
        int h;
        float r;
        int layer = 0;
        bool sortY = false;

        TransformComponent() = default;
        /*
            Transform Component
            @param x position X
            @param y position X
            @param w Width
            @param h Height
            @param r Rotation
        */
        TransformComponent(float _x, float _y, int _w, int _h, float _r = 0, int _layer = 0, bool sY = false)
            : x(_x), y(_y), w(_w), h(_h), r(-_r), layer(_layer), sortY(sY) { }

        bool operator < (TransformComponent& tran) const {
            if (sortY && tran.sortY) {
                return std::tie(layer, y) < std::tie(tran.layer, tran.y);
            }
            else {
                return layer < tran.layer;
            }
        }

        inline Vec4f getRect() { return { x, y, w, h }; }
        inline Vec2f getPosition() { return { x,y }; }
        inline Vec2i getSize() { return { w, h }; }
        inline Vec2i getOffset() { return { offsetX, offsetY }; }
        inline Vec2f getCenter() { return { x + (w >> 1), y + (h >> 1) }; }
        std::array<Vec2f, 4> getvertices() {
            return std::array<Vec2f, 4>{ { {x, y}, { x,y + h }, { x + w, y + h }, { x + w, y } }};
        }
    };

} // namespace Plutus