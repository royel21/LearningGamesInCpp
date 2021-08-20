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
    mTextLayer.Init(static_cast<float>(w), static_cast<float>(h), "assets/fonts/Zoika.ttf", 28);
    std::printf("font-loaded\n");
    mScene->Init(&mWorldCamera);
    // mInput = Plutus::Input::getInstance();

    // Plutus::SceneLoader::loadFromJson("assets/scene1.json", mScene);

    Plutus::Textures::get()->addTexture("bats", "assets/textures/monster/bat.png", 3, 32, 32);
    Plutus::Textures::get()->addTexture("Player", "assets/textures/player1.png");
    auto player = mScene->createEntity("Player");
    player->addComponent<Plutus::Transform>(256.0f, h - 32.0f, 32, 32, 0.0f);
    player->addComponent<Plutus::Sprite>("Player");
    player->addComponent<Plutus::Script>("assets/script/player1.lua", player, mScene.get());

    mEngine->setViewPort(w, h);

    auto bat = mScene->createEntity("Bat");
    bat->addComponent<Plutus::Transform>(288.0f, h - 64.0f, 64, 64, 0.0f);
    bat->addComponent<Plutus::Script>("assets/script/bat.lua", bat, mScene.get());
    bat->addComponent<Plutus::Sprite>("bats");

    auto& anim = bat->addComponent<Plutus::Animate>(bat);

    anim.addTexture("bats");
    anim.AddAnimation("up", { {0, 1, 2}, 0, 120 });
    anim.AddAnimation("left", { {3, 4, 5}, 0, 120 });
    anim.AddAnimation("down", { {6, 7, 8}, 0, 120 });
    anim.AddAnimation("right", { {9, 10, 11}, 0, 120 });

    anim.PlayAnimation("down");
}

void GameScreen::update(float dt)
{
    mWorldCamera.update();

    mScene->update(dt);
    // if (mInput->onKeyPressed("PageUp"))
    // {
    //     mCurrentState = Plutus::ScreenState::CHANGE_PREV;
    // }
    // if (mInput->onKeyPressed("PageDown"))
    // {
    //     mCurrentState = Plutus::ScreenState::CHANGE_NEXT;
    // }

    // if (mInput->onKeyDown("Ctrl") && mInput->getMouseWheel() != 0)
    // {
    //     std::printf("wheel: %i\n", mInput->getMouseWheel());
    // }
}

void GameScreen::draw()
{
    mScene->draw();
    char text[20];
    snprintf(text, 20, "%.1f FPS", mEngine->getFPS());
    mTextLayer.drawString(text, 5.0f, 5.0f, 1.0f, { 0, 0, 0.7f, 1 });
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
