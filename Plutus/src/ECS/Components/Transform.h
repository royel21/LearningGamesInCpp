#pragma once
#include <Math/Vectors.h>

namespace Plutus
{
    class Transform
    {
    public:
        float x;
        float y;
        int w;
        int h;
        float r;
        int layer = 0;
        bool sortY = false;

    public:
        Transform() = default;
        /*
            Transform Component
            @param x position X
            @param y position X
            @param w Width
            @param h Height
            @param r Rotation
        */
        Transform(float _x, float _y, int _w, int _h, float _r = 0, int _layer = 0, bool sY = false)
            : x(_x), y(_y), w(_w), h(_h), r(-_r), layer(_layer), sortY(sY)
        {
        }

        bool operator < (Transform& tran) const {
            if (sortY && tran.sortY) {
                return std::tie(layer, y) < std::tie(tran.layer, tran.y);
            }
            else {
                return layer < tran.layer;
            }
        }

        vec4f getRect() { return { x, y, w, h }; }
        vec2f getPosition() { return { x,y }; }
        vec2i getSize() { return { w, h }; }
    };

} // namespace Plutus