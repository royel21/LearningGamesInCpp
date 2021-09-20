#pragma once
#include "System.h"

namespace Plutus
{
    class ScriptSystem : public ISystem
    {
    public:
        ScriptSystem(Scene* scene, Camera2D* camera);
        void update(float dt);
    };
} // namespace Plutus