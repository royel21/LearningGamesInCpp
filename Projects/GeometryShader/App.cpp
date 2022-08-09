#include "App.h"

#include <glm/glm.hpp>


#include <Time/Timer.h>
#include <Graphics/Graphic.h>

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Systems/Systems.h>

#include "RendererSystem.h"

#include "TextComponent.h"

#include <Log/Logger.h>

#include <Input/Input.h>

#include <filesystem>
#include <Utils/FileIO.h>


namespace Plutus
{
    AppGeo::AppGeo() {
        Logger::warn("Found: %i", FileIO::exists("ZombiesGame/ZombiesGame.json"));
        mProject.load("ZombiesGame/ZombiesGame.json");
    }

    void AppGeo::Init()
    {
        mCamControl.setCamera(&mCamera);

        auto ent = mProject.scene->getEntityByName("Player");
        if (ent) {
            ent.addComponent<TextComponent>("Aventurina.ttf", "Text Component LAND", 16.0f, 20.0f, ColorRGBA8{ 255, 255, 0, 255 });
        }

        mCamera.init(mProject.vpWidth, mProject.vpHeight);
        mCamera.setScale(mProject.zoomLevel);
        mSysManager.setProject(&mProject);

        mSysManager.AddSystem<ScriptSystem>(&mCamera);
        mSysManager.AddSystem<PhysicSystem>();
        mSysManager.AddSystem<AnimationSystem>();
        mSysManager.AddSystem<RendererSystem>(&mCamera);

        mSysManager.init();
        mWindow.setVSYNC(1);
    }

    bool pause = true;

    void AppGeo::Update(float dt)
    {

        if (Input.onKeyPressed("Enter")) pause = !pause;

        if (pause) mSysManager.update(dt);

        mWindow.setTitle("fps: %.02f - dt:%.04f", getFPS(), dt);
    }

    void AppGeo::Draw()
    {
        mSysManager.draw();
    }

    void  AppGeo::Resize(int w, int h) {
        Graphic::resizeViewport(w, h);
    }
}