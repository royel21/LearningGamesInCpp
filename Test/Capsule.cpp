#include "Capsule.h"

namespace Plutus
{
    void Capsule::create(float _x, float _y, float _w, float _h) {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
    }

    Box2d Capsule::getBox() {
        float x2 = w * 0.5f;
        return { x, y + x2, w, h - w };
    }

    Circle2d Capsule::getBCircle() {
        float x2 = w * 0.5f;
        return { x + x2, y + x2, x2 };
    }

    Circle2d Capsule::getTCircle() {
        float x2 = w * 0.5f;
        return  { x + w - x2, y + h - x2, x2 };
    }
} // namespace Plutus
