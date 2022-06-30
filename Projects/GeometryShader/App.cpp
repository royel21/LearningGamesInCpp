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
#include <Math/PMath.h>

#include "RendererSystem.h"

#include <Graphics/DebugRenderer.h> 

#include <Assets/Assets.h>

#include "TextComponent.h"

#include <GLFW/glfw3.h>

namespace Plutus
{
    float SPEED = 1;
    void AppGeo::run()
    {
        init();
        mWindow.setVSYNC(1);
        mLimiter.setFPSLimiter(false);

        while (mWindow.isFinish())
        {


            // update(dt / 1000000.0f);

            auto dt = mLimiter.start();

            // auto mCamPos = mCamera.getPosition();
            // mCamPos.x += SPEED;
            // // auto rounded = PMath::round(mCamPos);
            // mCamera.setPosition(mCamPos);
            mCamera.update();
            update(dt);

            auto start = glfwGetTime();
            mWindow.update();
            // auto end = glfwGetTime();
            // float dt2 = float(end - start);
            // Logger::info("dt:%.05f dt2:%0.5f", dt, dt2);
            // mProject.scene->remove();

            mLimiter.end();
            mWindow.setTitle("fps: %.04f\n", dt);
        }
    }

    void AppGeo::init()
    {
        mWindow.init("Plutus AppGeo", 1280, 768);

        mDebug = DebugRender::get();
        mDebug->init(&mCamera);
        mCamControl.setCamera(&mCamera);

        mInput = Input::get();

        mProject.load("ZombiesGame/ZombiesGame.json");
        mProject.loadScene(mProject.currentScene);

        auto ent = mProject.scene->getEntityByName("Player");
        if (ent) {
            ent.addComponent<TextComponent>("arial.ttf", "Text Component LAND", 16.0f, 20.0f, ColorRGBA8{ 255, 255, 0, 255 });
        }

        mCamera.init(mProject.vpWidth, mProject.vpHeight);
        mCamera.setScale(mProject.zoomLevel);
        mSysManager.setProject(&mProject);

        mSysManager.AddSystem<ScriptSystem>(&mCamera);
        mSysManager.AddSystem<ScriptSystem>(&mCamera);
        mSysManager.AddSystem<PhysicSystem>();
        mSysManager.AddSystem<AnimationSystem>();
        mSysManager.AddSystem<RendererSystem>(&mCamera);

        mSysManager.init();

        mWindow.onResize = [&](int w, int h) { glViewport(0, 0, w, h);  };
    }

    void AppGeo::update(float dt)
    {
        mSysManager.update(dt);
    }

    void AppGeo::draw()
    {

    }
}