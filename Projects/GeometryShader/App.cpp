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
    double SPEED = 60;
    void AppGeo::run()
    {
        init();
        mWindow.setVSYNC(1);
        auto start = glfwGetTime();
        while (mWindow.isFinish())
        {
            auto end = glfwGetTime();
            auto dt = end - start;
            start = end;

            // update(dt / 1000000.0f);

            // auto dt = mLimiter.start();

            auto mCamPos = mCamera.getPosition();
            if (Input::get()->onKeyDown("Up")) {
                mCamPos.y += SPEED * dt;
            }
            if (Input::get()->onKeyDown("Down")) {
                mCamPos.y -= SPEED * dt;
            }
            if (Input::get()->onKeyDown("Right")) {
                mCamPos.x += SPEED * dt;
            }
            if (Input::get()->onKeyDown("Left")) {
                mCamPos.x -= SPEED * dt;
            }
            mCamPos.x += SPEED * dt;
            // auto rounded = PMath::round(mCamPos);
            mCamera.setPosition(mCamPos);
            Logger::info("X:%.0f Y:%0.3f dt:%0.5f", mCamPos.x, mCamPos.y, dt);

            mCamera.update();
            update((float)dt);
            draw();
            mWindow.update();
            mProject.scene->remove();

            // mLimiter.end();
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