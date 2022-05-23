#pragma once

#include <tuple>
#include <Math/Vectors.h>
#include <Utils/ColorRGBA8.h>
#include <Assets/Texture.h>

namespace Plutus
{
    struct Texture;

    struct Renderable {
        // Texture Id
        Texture* texture;
        //offsetY
        float offsetY;
        // Rectangle with position x,y and width, height
        Vec4f trans;
        // Texture coords "UV"
        Vec4f uv = { 0,0,1,1 };
        // Color
        ColorRGBA8 color = {};
        // Rotation
        float r = 0;
        // Flip the texture coord on x
        bool flipX = false;
        // Flip the texture coord on y
        bool flipY = false;
        // Entity Id for mouse picking
        int entId = -1;
        // Layer Id
        int layer = 0;
        // Sort in Y order
        bool sortY = false;
        Renderable() = default;
        Renderable(Texture* tex, Vec4f rect, Vec4f _uv, ColorRGBA8 _c, float offY = 0) : texture(tex), trans(rect), uv(_uv), color(_c), offsetY(offY) {}

        Renderable(Texture* tex, Vec4f rect, Vec4f _uv, ColorRGBA8 _c, float _r, bool fx, bool fy, int id, int _layer, bool sY = false, float offY = 0)
            : texture(tex), trans(rect), uv(_uv), color(_c), r(_r), flipX(fx), flipY(fy), entId(id), layer(_layer), sortY(sY), offsetY(offY) {
        }

        bool operator < (const Renderable& rend) const {
            if (sortY && rend.sortY) {
                const float y1 = trans.y + offsetY;
                const float y2 = rend.trans.y + rend.offsetY;
                return std::tie(layer, y2, texture->mTexId) < std::tie(rend.layer, y1, texture->mTexId);
            }
            else {
                return std::tie(layer, texture->mTexId) < std::tie(rend.layer, rend.texture->mTexId);
            }
        }
    };
} // namespace Plutus

