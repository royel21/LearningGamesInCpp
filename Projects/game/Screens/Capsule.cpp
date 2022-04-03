#include "Capsule.h"
#include <box2d/b2_body.h>
#include <Physics/Shapes.h>

namespace Plutus
{
    void Capsule::create(float x, float y, float w, float h) {
        position = { x, y };
        size = { w, h };
        radius = w * 0.5f;
    }

    Box2d Capsule::getBox() {
        return { position.x, position.y + radius, size.x, size.y - size.x };
    }

    Circle2d Capsule::getBCircle() {
        return { position.x + radius, position.y + radius, radius };
    }

    Circle2d Capsule::getTCircle() {
        return  { position.x + size.x - radius, position.y + size.y - radius, radius };
    }

    void Capsule::update()
    {
        auto pos = fromWorld(body->GetPosition());
        position.x = pos.x;
        position.y = pos.y - radius;
    }
} // namespace Plutus
