#include "Capsule.h"
#include <box2d/b2_body.h>
#include <Physics/Shapes.h>

namespace Plutus
{
    void Capsule::create(float _x, float _y, float _w, float _h) {
        position = { x,y };
        size = { w, h };
        x = _x;
        y = _y;
        w = _w;
        h = _h;
        radius = w * 0.5f;
    }

    Box2d Capsule::getBox() {
        return { position.x, position.y + radius, w, h - w };
    }

    Circle2d Capsule::getBCircle() {
        return { position.x + radius, position.y + radius, radius };
    }

    Circle2d Capsule::getTCircle() {
        return  { position.x + w - radius, position.y + h - radius, radius };
    }

    void Capsule::update()
    {
        auto pos = fromWorld(body->GetPosition());
        position.x = pos.x;
        position.y = pos.y - w * 0.5f;
    }
} // namespace Plutus
