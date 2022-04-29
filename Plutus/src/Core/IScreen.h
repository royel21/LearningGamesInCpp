#pragma once

#include "Core.h"
#include <ECS/Scene.h>
#include <Input/Input.h>

namespace Plutus
{
    class IScreen
    {
    protected:
        Core* mCore;
        Scene mScene;
        Input* mInput;
        bool mIsInitialized = false;
        Camera2D* mCamera = nullptr;

    public:
        ~IScreen() { Destroy(); }
        virtual void Init() { }
        virtual void Enter() {}
        virtual void Update(float dt) {}
        virtual void Draw() {}
        virtual void Resize(int width, int height) {}
        virtual void Exit() {}
        virtual void Destroy() {}

        bool isInizialized() { return mIsInitialized; }
    private:
        void init(Core* core, Camera2D* camera) {
            mCore = core;
            mCamera = camera;
            mInput = Input::get();
            mIsInitialized = true;

        };

        friend class Core;
    };
} // namespace Plutus