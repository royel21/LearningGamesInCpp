#pragma once
#include <Math/Vectors.h>

namespace Plutus
{
    class TransformComponent
    {
    public:
        float x;
        float y;
        float offsetX = 0;
        float offsetY = 0;
        int w;
        int h;
        float r;
        int layer = 0;
        bool sortY = false;

    public:
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

        Vec4f getRect() { return { x, y, w, h }; }
        Vec2f getPosition() { return { x,y }; }
        Vec2i getSize() { return { w, h }; }
        Vec2i getOffset() { return { offsetX, offsetY }; }
    };

} // namespace Plutus