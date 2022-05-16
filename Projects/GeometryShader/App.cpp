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

namespace Plutus
{
    void AppGeo::run()
    {
        init();
        while (mWindow.isFinish())
        {
            update();
            draw();
            mWindow.update();
        }
    }

    void AppGeo::init()
    {
        mWindow.init("Plutus AppGeo", 1280, 768);
        glClearColor(0.0f, 0.65f, .95f, 1.0f);
        mInput = Input::get();
        mSysManager.setProject(&mProject);

        mProject.load("ZombiesGame/ZombiesGame.json");
        mProject.loadScene(mProject.currentScene);

        mCamera.init(mProject.vpWidth, mProject.vpHeight);
        mCamera.setScale(mProject.zoomLevel);
        // mCamera2.init(1280, 768);

        mSysManager.AddSystem<ScriptSystem>(&mCamera);
        mSysManager.AddSystem<PhysicSystem>();
        mSysManager.AddSystem<AnimationSystem>();
        mSysManager.AddSystem<TileMapSystem>(&mCamera);
        mSysManager.AddSystem<RenderSystem>(&mCamera);
        // mMapRender.init(&mCamera);

        // auto mapView = mProject.scene->getRegistry()->view<TileMapComponent>();

        // for (auto [e, map] : mapView.each()) {
        //     auto ent = Entity{ e, mProject.scene.get() };
        //     mMapRender.addMap(ent.getName(), &map);
        // }

        mSysManager.init();
        // mCamera.setTarget({}, {});
    }

    void AppGeo::update()
    {
        auto pos = mCamera.getPosition();
        if (mInput->onKeyDown("Right"))
        {
            pos.x -= speed;
        }

        if (mInput->onKeyDown("Left"))
        {
            pos.x += speed;
        }

        if (mInput->onKeyDown("Up"))
        {
            pos.y -= speed;
        }

        if (mInput->onKeyDown("Down"))
        {
            pos.y += speed;
        }
        mCamera.setPosition(pos);

        auto scale = mCamera.getScale();
        if (mInput->onKeyDown("NUMPAD+"))
        {
            scale += 0.05f;
        }

        if (mInput->onKeyDown("NUMPAD-"))
        {
            scale -= 0.05f;
        }
        mCamera.setScale(scale);


        mCamera.update();

        auto start = Time::micros();
        mSysManager.update(0.01667f);
        Logger::info("elapse: %llu", Time::micros() - start);
    }

    void AppGeo::draw()
    {
        // glDisable(GL_BLEND);
        // mMapRender.draw();

        // if (mInput->onKeyPressed("MouseLeft")) {
        //     auto mpos = mInput->getMouseCoords();
        //     float pixels[4];
        //     glReadPixels(static_cast<int>(mpos.x), static_cast<int>(mpos.y), 1, 1, GL_RGBA, GL_FLOAT, pixels);
        //     Logger::info("values: %.6f, %.6f, %.6f, %.6f", pixels[0], pixels[1], pixels[2], pixels[3]);
        // }
    }
}