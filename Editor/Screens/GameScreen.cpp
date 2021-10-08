#include "GameScreen.h"

#include <cstdlib>
#include <ctime>

#include "Assets/AssetManager.h"

#include "ECS/Components.h"

#include "Core/Engine.h"
#include "Utils/Utils.h"
#include <time.h>
#include <Serialize/SceneLoader.h>
#include <Time/Timer.h>
#include <Systems/RenderSystem.h>
#include <Systems/ScriptSystem.h>
#include <Systems/AnimationSystem.h>


GameScreen::GameScreen()
{
}

GameScreen::~GameScreen()
{
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
    mEngine->setFPS(60);
    const int w = mEngine->getWidth();
    const int h = mEngine->getHeight();

    mWorldCamera.init(w, h);

    mScene = Plutus::CreateRef<Plutus::Scene>();

    mSystemManager.init(mScene.get());
    mSystemManager.AddSystem<Plutus::RenderSystem>(&mWorldCamera);
    mSystemManager.AddSystem<Plutus::ScriptSystem>(&mWorldCamera);
    mSystemManager.AddSystem<Plutus::AnimationSystem>();

    mInput = Plutus::Input::getInstance();
}

void GameScreen::onEntry()
{
    Plutus::SceneLoader::loadFromJson("assets/scenes/scene4.json", mScene);
    mSystemManager.start();
}

void GameScreen::update(float dt)
{
    mWorldCamera.update();

    if (mInput->onKeyPressed("PageUp"))
    {
        mCurrentState = Plutus::ScreenState::CHANGE_PREV;
    }
    if (mInput->onKeyPressed("PageDown"))
    {
        mCurrentState = Plutus::ScreenState::CHANGE_NEXT;
    }

    mSystemManager.update(dt);
}

void GameScreen::draw()
{
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
