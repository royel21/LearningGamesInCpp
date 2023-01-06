#pragma once
#include "System.h"

namespace Plutus
{
    class AnimationSystem : public ISystem
    {
    public:
        void init();
        void update(float dt);
    };
} // namespace Plutus