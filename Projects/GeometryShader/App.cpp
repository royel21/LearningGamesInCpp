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

#include <Graphics/DebugRenderer.h> 

#include <Assets/Assets.h>

#include "TextComponent.h"

namespace Plutus
{
    void AppGeo::run()
    {
        init();
        mWindow.setVSYNC(1);
        auto start = Time::micros();
        while (mWindow.isFinish())
        {
            auto end = Time::micros();
            auto dt = end - start;
            start = end;

            update(0.01667f);
            draw();
            mWindow.update();
            mProject.scene->remove();
            mWindow.setTitle("fps: %llu", dt);
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
            ent.addComponent<TextComponent>("Zoika.ttf", "Text Component LAND", 16.0f, 48.0f, ColorRGBA8{ 255, 255, 0, 255 });
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
        mCamera.update();
        mSysManager.update(dt);
    }

    void AppGeo::draw()
    {

    }
}