
#include "GameScreen.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <ECS/Components.h>

#include <Assets/AssetManager.h>
#include <Serialize/SceneLoader.h>
#include <Graphics/GLSL.h>

#include <Input/Input.h>
#include <time.h>
#include <Time/Timer.h>
#include <Core/Engine.h>
#include <Utils/Utils.h>
#include <Systems/Systems.h>

#include <Assets/SoundEngine.h>

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

    // auto& textures = Plutus::AssetManager::get()->mTextures;
    // textures.addTexture("bats", "assets/textures/monster/bat.png", 3, 32, 32);
    // textures.addTexture("player1", "assets/textures/player1.png");
    // textures.addTexture("player", "assets/textures/player.png", 60, 64, 64);
    // textures.addTexture("cave", "assets/textures/goblin_cave.png", 8, 32, 32);

    mSystemManager.setScene(mScene.get());
    mSystemManager.AddSystem<Plutus::ScriptSystem>(&mWorldCamera);
    mSystemManager.AddSystem<Plutus::PhysicSystem>();
    mSystemManager.AddSystem<Plutus::AnimationSystem>();
    mSystemManager.AddSystem<Plutus::RenderSystem>(&mWorldCamera);

    auto debugSys = mSystemManager.AddSystem<Plutus::DebugSystem>(&mWorldCamera);

    // debugSys->drawGrid(true);

    mScene->getWorld()->SetGravity({ 0.0f, 0 });
    mScene->getWorld()->SetAutoClearForces(true);
    // Plutus::SoundEngine.add("bg-sound", "assets/sounds/XYZ2.ogg", Plutus::MUSIC);
}

void GameScreen::onEntry()
{
    Plutus::SceneLoader::loadFromJson("assets/scenes/scene4.json", mScene.get());
    Player = mScene->getEntityByName("Player2");
    if (Player) {
        Player.addComponent<Plutus::Velocity>();

        auto pbody = Player.addComponent<Plutus::RigidBody>(Player, Plutus::DynamicBody);
        pbody->mOffset = { 26, 21 };
        pbody->addBox({ 0, 0 }, { 8,8 }, 2);
        pbody->linearDamping = 10;
        pbody->gravityScale = 0;
    }
    printf("player init");

    auto ground = mScene->getEntityByName("Floor");
    if (ground) {
        auto body = ground.addComponent<Plutus::RigidBody>(ground, Plutus::StaticBody);
        auto trans = ground.getComponent<Plutus::Transform>();

        body->addBox({ 0, 0 }, { (float)trans->w, (float)trans->h });
    }
    printf("ground init");


    // const int h = mEngine->getHeight();
    // // Player 1
    // auto player = mScene->createEntity("Player");
    // player.addComponent<Plutus::Transform>(192.0f, 0.0f, 64, 64, 0.0f, 0, true);
    // player.addComponent<Plutus::Sprite>("player1");
    // player.addComponent<Plutus::Script>("assets/script/player1.lua");

    // // Player 2
    // auto player2 = mScene->createEntity("Player2");
    // player2.addComponent<Plutus::Transform>(400.0f, 0.0f, 128, 128, 0.0f, 0, true);
    // player2.addComponent<Plutus::Sprite>("player2");
    // player2.addComponent<Plutus::Animation>();
    // player2.addComponent<Plutus::Script>("assets/script/player2.lua");
    // // Bat1
    // auto bat = mScene->createEntity("Bat");
    // bat.addComponent<Plutus::Transform>(288.0f, 0.0f, 64, 64, 0.0f, 0, true);
    // bat.addComponent<Plutus::Sprite>("bats");

    // bat.addComponent<Plutus::Animation>();
    // bat.addComponent<Plutus::Script>("assets/script/bat.lua");
    // Plutus::SoundEngine.play("bg-sound");

    mSystemManager.start();
}

void GameScreen::update(float dt)
{

    float cscale = mWorldCamera.getScale();

    if (mInput->onKeyPressed("R")) {
        mWorldCamera.setScale(1);
    }

    if (mInput->isCtrl) {

        if (mInput->onKeyDown("+")) {
            cscale += 0.05f;
            mWorldCamera.setScale(cscale);
        }
        if (mInput->onKeyDown("-")) {
            cscale -= 0.05f;
            mWorldCamera.setScale(cscale);
        }

        auto cPos = mWorldCamera.getPosition();
        if (mInput->onKeyDown("Right")) {
            cPos.x += 5;
        }
        if (mInput->onKeyDown("Left")) {
            cPos.x -= 5;
        }
        if (mInput->onKeyDown("Up")) {
            cPos.y += 5;
        }
        if (mInput->onKeyDown("Down")) {
            cPos.y -= 5;
        }
        mWorldCamera.setPosition(cPos);
    }
    else {
        auto trans = Player.getComponent<Plutus::Transform>();

        auto size = mWorldCamera.getScaleScreen();
        mWorldCamera.setPosition({ trans->x - (size.x * 0.5f), trans->y - (size.y * 0.15f) });
    }

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
    Plutus::SoundEngine.stop("bg-sound");
    mSystemManager.stop();
    mScene->clear();
}

void GameScreen::destroy()
{
    Plutus::SoundEngine.cleanUp();
}
