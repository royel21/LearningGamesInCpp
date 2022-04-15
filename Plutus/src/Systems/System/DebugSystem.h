#pragma once

#include "System.h"

namespace Plutus
{
    class DebugRender;

    class DebugSystem : public ISystem
    {
    public:
        DebugSystem(Camera2D* camera) : ISystem(camera) {};
        void init(Scene* scene);
        void update(float dt);
        void drawGrid(bool isDraw, int cellW = 32, int cellH = 32);
    private:
        DebugRender* mDebug;
        bool mIsDrawGrid = false;
    };
} // namespace Plutus