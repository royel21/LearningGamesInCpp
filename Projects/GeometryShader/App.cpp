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

#include <Systems/System/TileMapSystem.h>

#include <Log/Logger.h>

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


        mCamera.init(1280, 768);
        // mCamera2.init(1280, 768);
        mProject.load("ZombiesGame/ZombiesGame.json");
        mProject.loadScene(mProject.currentScene);

        mSysManager.AddSystem<TileMapSystem>(&mCamera);
        // mMapRender.init(&mCamera2);

        // auto mapView = mProject.scene->getRegistry()->view<TileMapComponent>();

        // for (auto [e, map] : mapView.each()) {
        //     auto ent = Entity{ e, mProject.scene.get() };
        //     mMapRender.addMap(ent.getName(), &map);
        // }

        mSysManager.init();
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
        mSysManager.update(0.01667f);
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