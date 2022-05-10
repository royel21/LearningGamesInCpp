#include "App.h"


#include <Graphics/Graphic.h>

#include <Time/Timer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Assets/Assets.h>

#include <Core/Project.h>
#include <ECS/Scene.h>
#include <ECS/Components.h>
#include <Serialize/SceneLoader.h>


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
        mInput = Input::get();

        mCamera.init(1280, 768);
        mProject.load("ZombiesGame/ZombiesGame.json");
        mProject.loadScene(mProject.currentScene);

        mMapRender.init(&mCamera);

        auto tilemap = mProject.scene->getEntityByName("floor");
        auto mapCom = tilemap.getComponent<TileMapComponent>();
        mMapRender.addMap("floor", mapCom);

        tilemap = mProject.scene->getEntityByName("bg1");
        mapCom = tilemap.getComponent<TileMapComponent>();
        mMapRender.addMap("bg1", mapCom);
    }

    void AppGeo::update()
    {
        auto pos = mCamera.getPos();
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
        mCamera.setPos(pos);

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
    }

    void AppGeo::draw()
    {
        glClearColor(0.0f, 0.65f, .95f, 1.0f);
        mMapRender.draw();
    }
}