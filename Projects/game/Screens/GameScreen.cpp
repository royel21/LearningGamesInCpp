
#include "GameScreen.h"

#include <ECS/Components.h>

#include <Log/Logger.h>
#include <Systems/Systems.h>
#include <Serialize/SceneLoader.h>
#include <Assets/AssetManager.h>


GameScreen::GameScreen()
{
}

GameScreen::~GameScreen()
{
    mSystemManager.cleanup();
}

void GameScreen::Init()
{
    mSystemManager.setProject(&mCore->mProject);
    mSystemManager.AddSystem<Plutus::ScriptSystem>(mCamera);
    mSystemManager.AddSystem<Plutus::PhysicSystem>();
    mSystemManager.AddSystem<Plutus::AnimationSystem>();
    mSystemManager.AddSystem<Plutus::RenderSystem>(mCamera);

    // auto debugSys = mSystemManager.AddSystem<Plutus::DebugSystem>(mCamera);
    // debugSys->drawGrid(true, 64, 64);
}

void GameScreen::Enter()
{
    // Plutus::SceneLoader::loadFromPath("assets/scenes/testing.json", mCore->mProject.scene.get());
    mSystemManager.init();
}

void GameScreen::Update(float dt)
{
    auto pos = mCamera->getPosition();
    if (mInput->onKeyPressed("PageDown"))
    {
        mCore->setNextScreen("Editor");
    }
    if (mInput->onKeyDown("Right"))
    {
        mCamera->setPosition({ pos.x - 5, pos.y });
    }

    if (mInput->onKeyDown("Left"))
    {
        mCamera->setPosition({ pos.x + 5, pos.y });
    }
    if (mInput->onKeyDown("Up"))
    {
        mCamera->setPosition({ pos.x, pos.y - 5 });
    }

    if (mInput->onKeyDown("Down"))
    {
        mCamera->setPosition({ pos.x, pos.y + 5 });
    }

    auto scale = mCamera->getScale();
    if (mInput->onKeyDown("+"))
    {
        mCamera->setScale(scale > 0.2f ? scale - 0.1f : 0.01f);
    }

    if (mInput->onKeyDown("-"))
    {
        mCamera->setScale(scale < 15.0f ? scale + 0.1f : 15.0f);
    }

    mSystemManager.update(dt);
}

void GameScreen::Draw()
{
    setBackgoundColor(0.33f, 0.33f, 0.33f, 1);
}


void GameScreen::Exit()
{
    mSystemManager.stop();
    mScene.clear();
}
