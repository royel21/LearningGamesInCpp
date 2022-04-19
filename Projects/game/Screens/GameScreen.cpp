
#include "GameScreen.h"

#include <ECS/Components.h>

#include <Log/Logger.h>
#include <Systems/Systems.h>
#include <Serialize/SceneLoader.h>


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

    auto debugSys = mSystemManager.AddSystem<Plutus::DebugSystem>(mCamera);
    debugSys->drawGrid(true, 64, 64);
}

void GameScreen::Enter()
{
    Plutus::SceneLoader::loadFromPath("assets/scenes/Physics.json", &mScene);
    mSystemManager.init();
}

void GameScreen::Update(float dt)
{
    if (mInput->onKeyPressed("PageDown"))
    {
        mCore->setNextScreen("Editor");
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
