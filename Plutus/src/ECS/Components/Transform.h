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
        Transform(float _x = 0, float _y = 0, int _w = 0, int _h = 0, float _r = 0) : x(_x), y(_y), w(_w), h(_h), r(-_r)
        {
        }

        glm::vec4 getRect() { return { x, y, w, h }; }
        glm::vec2 getPosition() { return { x,y }; }
        glm::ivec2 getSize() { return { w, h }; }
    };

} // namespace Plutus