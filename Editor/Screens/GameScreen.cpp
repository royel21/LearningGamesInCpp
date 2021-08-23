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
}

void GameScreen::onEntry()
{
    mEngine->setFPS(60);
    const int w = mEngine->getWidth();
    const int h = mEngine->getHeight();

    mScene = Plutus::CreateRef<Plutus::Scene>();
    // mSystemManager.init(mScene.get());
    // mSystemManager.AddSystem<Plutus::RenderSystem>();
    mWorldCamera.init(w, h);
    mScene->Init(&mWorldCamera);
    mTextLayer.Init(static_cast<float>(w), static_cast<float>(h), "assets/fonts/Zoika.ttf", 28);

    mInput = Plutus::Input::getInstance();
    auto start = Plutus::Timer::millis();
    Plutus::SceneLoader::loadFromJson("assets/scene1.json", mScene);
}

void GameScreen::update(float dt)
{
    mWorldCamera.update();

    mScene->update(dt);
    if (mInput->onKeyPressed("PageUp"))
    {
        mCurrentState = Plutus::ScreenState::CHANGE_PREV;
    }
    if (mInput->onKeyPressed("PageDown"))
    {
        mCurrentState = Plutus::ScreenState::CHANGE_NEXT;
    }
    // mSystemManager.update(dt);
}

void GameScreen::draw()
{
    mScene->draw();
    char text[20];
    snprintf(text, 20, "%.1f FPS", mEngine->getFPS());
    mTextLayer.drawString(text, 5.0f, 5.0f, 1.0f, {1, 0, 0, 1});
    mTextLayer.drawString("This is a pretty large text for testing purpose and some more test", 5.0f, 30.0f);
    glClearColor(0, 0, 0, 1);
}

void GameScreen::onScreenResize(int w, int h)
{
}

void GameScreen::onExit()
{
}

void GameScreen::destroy()
{
}
