#include "App.h"

#include <glm/glm.hpp>


#include <Time/Timer.h>
#include <Graphics/Graphic.h>
#include <glm/gtc/matrix_transform.hpp>


#include <ECS/Scene.h>
#include <Core/Project.h>
#include <Assets/Assets.h>
#include <ECS/Components.h>
#include <Serialize/SceneLoader.h>

#include <Systems/Systems.h>

#include <Log/Logger.h>
#include <Time/Timer.h>

#include "RendererSystem.h"

// #include "SpriteBatch.h"

namespace Plutus
{
    void AppGeo::run()
    {
        init();
        while (mWindow.isFinish())
        {
            mLimiter.start();
            update();
            draw();
            mWindow.update();
            mLimiter.end();
        }
    }

    void AppGeo::init()
    {
        mCamControl.setCamera(&mCamera);

        mWindow.init("Plutus AppGeo", 1280, 768);
        glClearColor(0.0f, 0.65f, .95f, 1.0f);
        mInput = Input::get();

        mProject.load("ZombiesGame/ZombiesGame.json");
        mProject.loadScene(mProject.currentScene);

        mCamera.init(mProject.vpWidth, mProject.vpHeight);
        mCamera.setScale(mProject.zoomLevel);
        mSysManager.setProject(&mProject);
        // mCamera2.init(1280, 768);

        mSysManager.AddSystem<RendererSystem>(&mCamera);
        mSysManager.AddSystem<ScriptSystem>(&mCamera);
        mSysManager.AddSystem<PhysicSystem>();
        mSysManager.AddSystem<AnimationSystem>();
        mSysManager.AddSystem<DebugSystem>(&mCamera);

        mSysManager.init();
    }

    void AppGeo::update()
    {
        mCamera.update();
        auto pos = mCamera.getPosition();

        auto start = Time::micros();
        mSysManager.update(0.01667f);
        Logger::info("elapse: %llu", Time::micros() - start);


        // char title[128];
        // std::snprintf(title, 128, "FPS: %.2f", (Time::micros() - start) / 1000.0f);
        // mWindow.setTitle(title);
        // Logger::info("pos: %0.3f, %0.3f", pos.x, pos.y);
    }

    void AppGeo::draw()
    {

    }
}