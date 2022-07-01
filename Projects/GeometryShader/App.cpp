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


#include <filesystem>


namespace Plutus
{
    AppGeo::AppGeo() {
        mProject.load("ZombiesGame/ZombiesGame.json");
    }

    void AppGeo::Init()
    {
        mCamControl.setCamera(&mCamera);

        auto ent = mProject.scene->getEntityByName("Player");
        if (ent) {
            ent.addComponent<TextComponent>("arial.ttf", "Text Component LAND", 16.0f, 20.0f, ColorRGBA8{ 255, 255, 0, 255 });
        }

        mCamera.init(mProject.vpWidth, mProject.vpHeight);
        mCamera.setScale(mProject.zoomLevel);
        mSysManager.setProject(&mProject);

        mSysManager.AddSystem<ScriptSystem>(&mCamera);
        mSysManager.AddSystem<PhysicSystem>();
        mSysManager.AddSystem<AnimationSystem>();
        mSysManager.AddSystem<RendererSystem>(&mCamera);

        mSysManager.init();
    }

    void AppGeo::Update(float dt)
    {
        mSysManager.update(dt);
        mWindow.setTitle("fps: %.04f\n", dt);
    }

    void AppGeo::Draw()
    {
    }

    void  AppGeo::Resize(int w, int h) {
        Graphic::resizeViewport(w, h);
    }
}