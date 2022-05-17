#pragma once

#include "Vectors.h"

namespace Plutus
{
    struct Rect {
        Vec2f pos;
        Vec2f size;
        Rect() {};
        Rect(float x, float y, float w, float h) : pos(x, y), size(w, h) {}
        Rect(const Vec2f& p, const Vec2f& s) : pos(p), size(s) {}

        bool contains(const Vec2f& p) const { return !(p.x < pos.x || p.y < pos.y || p.x >= (pos.x + size.x) || p.y >= (pos.y + size.y)); }

        bool contains(const Rect& r) const {
            return (r.pos.x >= pos.x) && (r.pos.x + r.size.x < pos.x + size.x) &&
                (r.pos.y >= pos.y) && (r.pos.y + r.size.y < pos.y + size.y);
        }

        bool overlaps(const Rect& r) const {
            return (pos.x < r.pos.x + r.size.x && pos.x + size.x >= r.pos.x && pos.y < r.pos.y + r.size.y && pos.y + size.y >= r.pos.y);
        }

        inline Vec4f getBox() const { return { pos, size }; }
    };
} // namespace Plutus
