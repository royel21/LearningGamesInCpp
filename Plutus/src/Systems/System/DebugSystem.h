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
        void drawGrid(bool isDraw, int cellW = 32, int cellH = 32);
    private:
        DebugRender* mDebug;
        bool mIsDrawGrid = false;
    };
} // namespace Plutus