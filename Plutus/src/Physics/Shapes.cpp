#include "Shapes.h"
#include <box2d/b2_body.h>

namespace Plutus
{
    void Shape::update() {
        pos = fromWorld(body->GetPosition());
    }
} // namespace Plutus
