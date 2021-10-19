
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

#include <Assets/SoundEngine.h>

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

    auto& textures = Plutus::AssetManager::get()->mTextures;
    textures.addTexture("bats", "assets/textures/monster/bat.png", 3, 32, 32);
    textures.addTexture("player1", "assets/textures/player1.png");
    textures.addTexture("player2", "assets/textures/player.png", 60, 64, 64);
    textures.addTexture("cave", "assets/textures/goblin_cave.png", 8, 32, 32);

    mSystemManager.init(mScene.get());
    mSystemManager.AddSystem<Plutus::RenderSystem>(&mWorldCamera);
    mSystemManager.AddSystem<Plutus::ScriptSystem>(&mWorldCamera);
    mSystemManager.AddSystem<Plutus::AnimationSystem>();

    Plutus::SoundEngine.add("bg-sound", "assets/sounds/XYZ2.ogg", Plutus::MUSIC);
}

void GameScreen::onEntry()
{
    // Plutus::SceneLoader::loadFromJson("assets/scenes/Physics.json", mScene);
    mSystemManager.start();
    const int h = mEngine->getHeight();
    // Player 1
    auto player = mScene->createEntity("Player");
    auto trasn1 = player.addComponent<Plutus::Transform>(192.0f, 0.0f, 64, 64, 0.0f, 0, true);
    player.addComponent<Plutus::Sprite>("player1");
    auto script = player.addComponent<Plutus::Script>();
    script->init("assets/script/player1.lua", player, mScene.get());

    // Player 2
    auto player2 = mScene->createEntity("Player2");
    player2.addComponent<Plutus::Transform>(400.0f, 0.0f, 128, 128, 0.0f, 0, true);
    player2.addComponent<Plutus::Sprite>("player2");

    auto anim2 = player2.addComponent<Plutus::Animation>();
    anim2->addTexture("player2");

    auto player2Script = player2.addComponent<Plutus::Script>();
    player2Script->init("assets/script/player2.lua", player2, mScene.get());
    // Bat1
    auto bat = mScene->createEntity("Bat");
    bat.addComponent<Plutus::Transform>(288.0f, 0.0f, 64, 64, 0.0f, 0, true);
    bat.addComponent<Plutus::Sprite>("bats");

    auto anim = bat.addComponent<Plutus::Animation>();
    anim->addTexture("bats");
    auto batScript = bat.addComponent<Plutus::Script>();
    batScript->init("assets/script/bat.lua", bat, mScene.get());
    Plutus::SoundEngine.play("bg-sound");
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
    Plutus::SoundEngine.stop("bg-sound");
    mSystemManager.stop();
    mScene->clear();
}

void GameScreen::destroy()
{
    Plutus::SoundEngine.cleanUp();
}
