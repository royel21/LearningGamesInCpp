#pragma once
#include <glm/glm.hpp>

namespace Plutus
{
    struct RigidBody2d
    {
        glm::vec2 position;
        float rotation = 0.0f;
    };
} // namespace Plutus
