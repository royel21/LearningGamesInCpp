#pragma once
#include <Core.h>
#include <Physics/Shapes.h>

namespace Plutus
{
    class DebugRender;
    class App : public Core
    {
    public:
        App();
        App(const char* name, int width, int height);
        ~App();
        void Setup();
        void Update(float dt);
        void Draw();
        void Exit();

    private:
        Line line1;
        Line line2;
        DebugRender* mDebug;
    };
} // namespace Plutus