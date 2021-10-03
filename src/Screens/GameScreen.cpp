
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
    mSystemManager.start();
    const int h = mEngine->getHeight();
    // Player 1
    auto player = mScene->createEntity("Player");
    auto trasn1 = player.addComponent<Plutus::Transform>(192.0f, 0.0f, 64, 64, 0.0f, 0, true);
    player.addComponent<Plutus::Sprite>("player1");
    player.addComponent<Plutus::Script>("assets/script/player1.lua", player, mScene.get());

    // Player 2
    auto player2 = mScene->createEntity("Player2");
    player2.addComponent<Plutus::Transform>(400.0f, 0.0f, 128, 128, 0.0f, 0, true);
    player2.addComponent<Plutus::Sprite>("player2");

    auto& anim2 = player2.addComponent<Plutus::Animation>(player2);
    anim2.addTexture("player2");

    player2.addComponent<Plutus::Script>("assets/script/player2.lua", player2, mScene.get());

    // Bat1
    auto bat = mScene->createEntity("Bat");
    auto trasn3 = bat.addComponent<Plutus::Transform>(288.0f, 0.0f, 64, 64, 0.0f, 0, true);
    bat.addComponent<Plutus::Sprite>("bats");

    auto& anim = bat.addComponent<Plutus::Animation>(bat);
    anim.addTexture("bats");
    bat.addComponent<Plutus::Script>("assets/script/bat.lua", bat, mScene.get());


    // auto bed = mScene->createEntity("bed");
    // bed->addComponent<Plutus::Transform>(128.0f, h - 256.0f, 128, 128);
    // auto& sprite = bed->addComponent<Plutus::Sprite>("cave");
    // sprite.mUVCoord = Plutus::AssetManager::get()->getTexCoords("cave", { 1, 10, 96, 96 });
    // auto ent = mScene->getEntityByName("Player");
    // if (ent) {
    //     std::printf("name: %s", ent.getName().c_str());
    // }
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
    setBackgoundColor(1, 0, 0, 1);
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
