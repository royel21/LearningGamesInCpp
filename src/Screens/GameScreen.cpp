#include "GameScreen.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <Assets/Textures.h>
#include <Serialize/SceneLoader.h>
#include <ECS/Components.h>

#include <Core/Engine.h>
#include <Utils/Utils.h>
#include <time.h>
#include <Time/Timer.h>

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
    const int w = mEngine->getWidth();
    const int h = mEngine->getHeight();
    mEngine->setFPS(60);

    mScene = Plutus::CreateRef<Plutus::Scene>();
    mWorldCamera.init(w, h);

    std::printf("font-loaded\n");
    mScene->Init(&mWorldCamera);
    mInput = Plutus::Input::getInstance();

    // Plutus::SceneLoader::loadFromJson("assets/scene1.json", mScene);

    Plutus::Textures::get()->addTexture("bats", "assets/textures/monster/bat.png", 3, 32, 32);
    Plutus::Textures::get()->addTexture("Player", "assets/textures/player1.png");
    Plutus::Textures::get()->addTexture("cave", "assets/textures/goblin_cave.png", 8, 32, 32);

    auto player = mScene->createEntity("Player");
    player->addComponent<Plutus::Transform>(256.0f, h - 32.0f, 32, 32, 0.0f);
    player->addComponent<Plutus::Sprite>("Player");
    player->addComponent<Plutus::Script>("assets/script/player1.lua", player, mScene.get());

    mEngine->setViewPort(w, h);

    auto bat = mScene->createEntity("Bat");
    bat->addComponent<Plutus::Transform>(288.0f, h - 64.0f, 64, 64, 0.0f);
    bat->addComponent<Plutus::Script>("assets/script/bat.lua", bat, mScene.get());
    bat->addComponent<Plutus::Sprite>("bats");

    auto& anim = bat->addComponent<Plutus::Animation>(bat);

    anim.addTexture("bats");
    anim.AddSequence("up", { {0, 1, 2}, 0, 120 });
    anim.AddSequence("left", { {3, 4, 5}, 0, 120 });
    anim.AddSequence("down", { {6, 7, 8}, 0, 120 });
    anim.AddSequence("right", { {9, 10, 11}, 0, 120 });

    anim.PlaySequence("down");

    auto bed = mScene->createEntity("bed");
    bed->addComponent<Plutus::Transform>(128.0f, h - 256.0f, 128, 128);
    auto& sprite = bed->addComponent<Plutus::Sprite>("cave");
    auto uv = Plutus::Textures::get()->getTexture("cave")->getUV(1, 10, 96, 96);
    sprite.mUVCoord = uv;
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

    // if (mInput->onKeyDown("Ctrl") && mInput->getMouseWheel() != 0)
    // {
    //     std::printf("wheel: %i\n", mInput->getMouseWheel());
    // }
}

void GameScreen::draw()
{
    setBackgoundColor(1, 0, 0, 1);
    mScene->draw();
    // char text[20];
    // snprintf(text, 20, "%.1f FPS", mEngine->getFPS());
    // mTextLayer.drawString(text, 5.0f, 5.0f, 1.0f, { 0, 0, 0.7f, 1 });
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
