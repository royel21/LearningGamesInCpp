#pragma once
#include <Core/Core.h>

#include <ECS/Scene.h>
#include <Utils/Pointer.h>
#include <Systems/SystemManager.h>
#include <Graphics/FrameBuffer.h>

namespace Plutus
{
    class DebugRender;
    class SpriteBatch2D;

    class App : public Core
    {
    public:
        App(const char* name, int width, int height);

        void Init() override;

        void Update(float) override;
        //Called on the loop draw, should be overrided.
        void Draw() override;

        void Resize(int w, int h)override;

        void Exit() override;

    private:
        Scene* mScene;
        Entity mPlayer;
        FrameBuffer mFBuffer;
        SpriteBatch2D* mBatch;
        SystemManager mSystemManager;
    };
} // namespace Plutus
