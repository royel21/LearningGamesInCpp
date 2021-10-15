#include "Shapes.h"
#include <box2d/b2_body.h>

namespace Plutus
{
    void Shape::update() {
        if (type != PLine)
            pos = fromWorld(body->GetPosition());
    }
} // namespace Plutus
