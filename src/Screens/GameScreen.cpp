
#include "GameScreen.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <Assets/AssetManager.h>
#include <Serialize/SceneLoader.h>
#include <ECS/Components.h>
#include <Graphics/GLSL.h>

#include <Core/Engine.h>
#include <Utils/Utils.h>
#include <time.h>
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

    mScene = Plutus::CreateRef<Plutus::Scene>();
    const int w = mEngine->getWidth();
    const int h = mEngine->getHeight();
    mEngine->setFPS(60);

    mWorldCamera.init(w, h);

    // Plutus::SceneLoader::loadFromJson("assets/scene1.json", mScene);
    auto& textures = Plutus::AssetManager::get()->mTextures;
    textures.addTexture("bats", "assets/textures/monster/bat.png", 3, 32, 32);
    textures.addTexture("player1", "assets/textures/player1.png");
    textures.addTexture("player2", "assets/textures/player.png", 60, 64, 64);
    textures.addTexture("cave", "assets/textures/goblin_cave.png", 8, 32, 32);

    mSystemManager.init(mScene.get());
    mSystemManager.AddSystem<Plutus::RenderSystem>(&mWorldCamera);
    mSystemManager.AddSystem<Plutus::ScriptSystem>(&mWorldCamera);
    mSystemManager.AddSystem<Plutus::AnimationSystem>();
}

void GameScreen::onEntry()
{
    const int h = mEngine->getHeight();

    auto player = mScene->createEntity("Player");
    player->addComponent<Plutus::Transform>(192.0f, h - 70.0f, 64, 64, 0.0f);
    player->addComponent<Plutus::Sprite>("player1");
    player->addComponent<Plutus::Script>("assets/script/player1.lua", player, mScene.get());
    //Player
    auto player2 = mScene->createEntity("Player2");
    player2->addComponent<Plutus::Transform>(400.0f, h - 128.0f, 128, 128, 0.0f);
    player2->addComponent<Plutus::Sprite>("player2");
    player2->addComponent<Plutus::Script>("assets/script/player2.lua", player2, mScene.get());

    auto& anim2 = player2->addComponent<Plutus::Animation>(player2);
    anim2.addTexture("player2");

    anim2.addSequence("stand-u", { { 6 }, 0, 100 });
    anim2.addSequence("run-up", { {7, 8, 9, 10}, 0, 100 });

    anim2.addSequence("stand-d", { { 18 }, 0, 100 });
    anim2.addSequence("run-down", { {19, 20, 21, 22, 23}, 0, 100 });

    anim2.addSequence("stand-r", { { 0 }, 0, 100 });
    anim2.addSequence("run-right", { {1, 2,3,4,5}, 0, 100 });

    anim2.addSequence("stand-l", { { 12 }, 0, 100 });
    anim2.addSequence("run-left", { {13, 14,15,16}, 0, 100 });


    anim2.addSequence("attack-right", { {24, 25, 26, 27}, 0, 150 });
    anim2.addSequence("attack-up", { {28, 29, 30, 31}, 0, 60 });
    anim2.addSequence("attack-left", { {32, 33, 34, 35}, 0, 60 });
    anim2.addSequence("attack-down", { {36, 37, 38, 39}, 0, 60 });


    anim2.addSequence("jump-right", { {40, 41, 42, 43, 44}, 0, 90 });
    anim2.addSequence("jump-up", { { 45, 46, 47, 48, 49}, 0, 90 });
    anim2.addSequence("jump-left", { {50, 51, 52, 53, 54}, 0, 90 });
    anim2.addSequence("jump-down", { { 55, 56, 57, 58, 59}, 0, 90 });


    anim2.play("stand-r");
    anim2.mState = "standing";
    //Player2

    auto bat = mScene->createEntity("Bat");
    bat->addComponent<Plutus::Transform>(288.0f, h - 64.0f, 64, 64, 0.0f);
    bat->addComponent<Plutus::Script>("assets/script/bat.lua", bat, mScene.get());
    bat->addComponent<Plutus::Sprite>("bats");

    auto& anim = bat->addComponent<Plutus::Animation>(bat);

    anim.addTexture("bats");
    anim.addSequence("up", { {0, 1, 2}, 0, 120 });
    anim.addSequence("left", { {3, 4, 5}, 0, 120 });
    anim.addSequence("down", { {6, 7, 8}, 0, 120 });
    anim.addSequence("right", { {9, 10, 11}, 0, 120 });

    anim.play("down");

    auto bed = mScene->createEntity("bed");
    bed->addComponent<Plutus::Transform>(128.0f, h - 256.0f, 128, 128);
    auto& sprite = bed->addComponent<Plutus::Sprite>("cave");
    sprite.mUVCoord = Plutus::AssetManager::get()->getTexCoords("cave", { 1, 10, 96, 96 });
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
    setBackgoundColor(1, 0, 0, 1);
}

void GameScreen::onScreenResize(int w, int h)
{
}

void GameScreen::onExit()
{
    mScene->clear();
}

void GameScreen::destroy()
{
}
