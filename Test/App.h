#pragma once
#include <Core.h>
#include <Physics/Shapes.h>
#include <box2d/box2d.h>
#include <vector>

#include <memory>

namespace Plutus
{
    class DebugRender;
    class App : public Core
    {
    public:
        App() = default;
        App(const char* name, int width, int height);
        ~App();
        void Setup() override;
        void Update(float dt) override;
        void Draw() override;
        void Exit() override;

        Box2d* createBox(float x, float y, float w, float h, int type = 0, float friction = 1, ShapeType shape = Box);

    private:
        float timeStep = 1 / 60.0f;
        int32_t velIter = 6;
        int32_t posIter = 2;
        DebugRender* mDebug;
        std::unique_ptr<b2World> mWorld;
        std::vector<Box2d> mBoxes;
    };
} // namespace Plutus