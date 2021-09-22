#pragma once
#include "System.h"

namespace Plutus
{
    class AnimationSystem : public ISystem
    {
    public:
        AnimationSystem(Scene* scene) : ISystem(scene) {}
        void update(float dt);

    };
} // namespace Plutus