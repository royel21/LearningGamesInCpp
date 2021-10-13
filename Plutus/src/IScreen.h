#pragma once
#include <ECS/Scene.h>
#include <Input/Input.h>

namespace Plutus
{
    class Core;

    class IScreen
    {
    protected:
        Core* mCore;
        Scene mScene;
        Input* mInput;
        bool mIsInitialized = false;

    public:
        virtual void Init() { mIsInitialized = true; }
        virtual void Enter() {}
        virtual void update(float dt) {}
        virtual void draw() {}
        virtual void Resize(int width, int height) {}
        virtual void Exit() {}
        virtual void Destroy() {}

        bool isInizialized() { return mIsInitialized; }
    private:
        void loadScene();
        void init(Core* core) {
            mCore = core;
            mInput = Input::get();
        };

        friend class Core;
    };
} // namespace Plutus