
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
}

void GameScreen::Enter()
{
    mSystemManager.init();
}

float speed = 1;

void GameScreen::Update(float dt)
{
    auto pos = mCamera->getPosition();
    if (mInput->onKeyPressed("PageDown"))
    {
        mCore->setNextScreen("Editor");
    }

    if (mInput->onKeyDown("Right"))
    {
        mCamera->setPosition({ pos.x + speed, pos.y });
    }

    if (mInput->onKeyDown("Left"))
    {
        mCamera->setPosition({ pos.x - speed, pos.y });
    }

    if (mInput->onKeyDown("Up"))
    {
        mCamera->setPosition({ pos.x, pos.y + speed });
    }

    if (mInput->onKeyDown("Down"))
    {
        mCamera->setPosition({ pos.x, pos.y - speed });
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
    mSystemManager.draw();
}


void GameScreen::Exit()
{
    mSystemManager.stop();
    mScene.clear();
}
