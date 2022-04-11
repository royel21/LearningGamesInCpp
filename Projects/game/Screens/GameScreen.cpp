
#include "GameScreen.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <ECS/Components.h>

#include <Serialize/SceneLoader.h>
#include <Graphics/GLSL.h>

#include <Input/Input.h>
#include <time.h>
#include <Time/Timer.h>
#include <Core/Engine.h>
#include <Utils/Utils.h>
#include <Systems/Systems.h>

#include <Log/Logger.h>

GameScreen::GameScreen()
{
}

GameScreen::~GameScreen()
{
    mSystemManager.cleanup();
}

int GameScreen::getNextScreenIndex() const
{
    return 0;
}

int GameScreen::getPrevScreentIndex() const
{
    return 0;
}

void GameScreen::build()
{
    mScene = CreateRef<Plutus::Scene>();
    const int w = mEngine->getWidth();
    const int h = mEngine->getHeight();
    mEngine->setFPS(60);

    mWorldCamera.init(640, 520);

    mSystemManager.setScene(mScene.get());
    mSystemManager.AddSystem<Plutus::ScriptSystem>(&mWorldCamera);
    mSystemManager.AddSystem<Plutus::PhysicSystem>();
    mSystemManager.AddSystem<Plutus::AnimationSystem>();
    mSystemManager.AddSystem<Plutus::RenderSystem>(&mWorldCamera);

    auto debugSys = mSystemManager.AddSystem<Plutus::DebugSystem>(&mWorldCamera);
    debugSys->drawGrid(true);
}

void GameScreen::onEntry()
{
    Plutus::SceneLoader::loadFromJson("assets/scenes/Physics.json", mScene.get());
    mSystemManager.start();
}

void GameScreen::update(float dt)
{
    if (mInput->onKeyPressed("PageUp"))
    {
        mCurrentState = Plutus::ScreenState::CHANGE_PREV;
    }

    if (mInput->onKeyPressed("PageDown"))
    {
        mCurrentState = Plutus::ScreenState::CHANGE_NEXT;
    }

    mWorldCamera.update();
    mSystemManager.update(dt);
}

void GameScreen::draw()
{
    setBackgoundColor(0.33f, 0.33f, 0.33f, 1);
}

void GameScreen::onScreenResize(int w, int h)
{
}

void GameScreen::onExit()
{
    mSystemManager.stop();
    mScene->clear();
}

void GameScreen::destroy()
{
}
