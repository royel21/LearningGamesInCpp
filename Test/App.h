#pragma once
#include <Core.h>
#include <Physics/Shapes.h>
#include <box2d/box2d.h>
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

    private:
        Line line1;
        Line line2;
        DebugRender* mDebug;
        std::unique_ptr<b2World> mWorld;
        float timeStep = 1 / 60.0f;
        int32_t velIter = 8;
        int32_t posIter = 3;
    };
} // namespace Plutus