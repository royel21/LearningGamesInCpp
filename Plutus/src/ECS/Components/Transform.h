#pragma once
#include <glm/glm.hpp>

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
        Transform(float _x = 0, float _y = 0, int _w = 0, int _h = 0, float _r = 0, int _layer = 0, bool sY = false)
            : x(_x), y(_y), w(_w), h(_h), r(-_r), layer(_layer), sortY(sY)
        {
        }

        // copy constructor 
        Transform(const Transform& tran) {
            x = tran.x;
            y = tran.y;
            w = tran.w;
            h = tran.h;
            r = tran.r;
            layer = tran.layer;
            sortY = tran.sortY;
        }

        bool operator < (Transform& tran) const {
            if (sortY && tran.sortY) {
                return std::tie(layer, y) < std::tie(tran.layer, tran.y);
            }
            else {
                return layer < tran.layer;
            }
        }

        glm::vec4 getRect() { return { x, y, w, h }; }
        glm::vec2 getPosition() { return { x,y }; }
        glm::ivec2 getSize() { return { w, h }; }
    };

} // namespace Plutus