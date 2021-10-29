#pragma once
#include "System.h"

namespace Plutus
{
    class AnimationSystem : public ISystem
    {
    public:
        AnimationSystem(Scene* scene) : ISystem(scene) {}
        void init();
        void update(float dt);
    };
} // namespace Plutus