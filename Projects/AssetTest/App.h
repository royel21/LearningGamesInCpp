#pragma once
#include <Core.h>

#include <ECS/Scene.h>
#include <Utils/Pointer.h>
#include <Systems/SystemManager.h>


namespace Plutus
{
    class DebugRender;
    class App : public Core
    {
    public:
        App(const char* name, int width, int height) : Core(name, width, height) {}

        void Setup() override;

        void Update(float) override;

        void Resize(int w, int h)override;
        //Called on the loop draw, should be overrided.
        void Draw() override;

        void Exit() override;
    private:
        Ref<Scene> mScene;
        Entity mPlayer;

        SystemManager mSystemManager;
    };
} // namespace Plutus
