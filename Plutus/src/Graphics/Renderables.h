#include <tuple>
#include <Math/Vectors.h>
#include <Utils/ColorRGBA8.h>

namespace Plutus
{
    struct Renderable {
        // Texture Id
        uint32_t TexId;
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
        Renderable(uint32_t texture, Vec4f rect, Vec4f _uv, ColorRGBA8 _c) : TexId(texture), trans(rect), uv(_uv), color(_c) {}

        Renderable(uint32_t texture, Vec4f rect, Vec4f _uv, ColorRGBA8 _c, float _r, bool fx, bool fy, int id, int _layer, bool sY = false)
            : TexId(texture), trans(rect), uv(_uv), color(_c), r(_r), flipX(fx), flipY(fy), entId(id), layer(_layer), sortY(sY) {
        }

        bool operator < (const Renderable& rend) const {
            if (sortY && rend.sortY) {
                return std::tie(layer, rend.trans.y, TexId) < std::tie(rend.layer, trans.y, TexId);
            }
            else {
                return std::tie(layer, TexId) < std::tie(rend.layer, rend.TexId);
            }
        }
    };
} // namespace Plutus

