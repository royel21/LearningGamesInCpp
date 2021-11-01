#pragma once

#include "System.h"

namespace Plutus
{
    class DebugRender;

    class DebugSystem : public ISystem
    {
    public:
        DebugSystem(Scene* scene, Camera2D* camera) : ISystem(scene, camera) {};
        void init();
        void update(float dt);
    private:
        DebugRender* mDebug;
    };
} // namespace Plutus