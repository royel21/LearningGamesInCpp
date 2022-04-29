#pragma once
#include <Core/Core.h>
#include <Input/InputListener.h>
#include <Physics/Shapes.h>

namespace Plutus
{
    class DebugRender;

    class App : public Core, public InputListener
    {
    public:
        App() = default;
        App(const char* name, int width, int height);

        void Init() override;
        void Update(float dt) override;
        void Draw() override;
        void Exit() override;

        void onKeyDown(const std::string& key);
        void onKeyUp(const std::string& key);
        void onMouseMove(float x, float y);

    private:
        DebugRender* mDebug;
        std::vector<Shape*> shapes;
    };
} // namespace Plurus