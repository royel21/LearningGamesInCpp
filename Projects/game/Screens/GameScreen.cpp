
#include "GameScreen.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

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
    Player = mScene->getEntityByName("Player2");
    if (Player) {
        Player.addComponent<Plutus::VelocityComponent>();

        auto pbody = Player.addComponent<Plutus::RigidBodyComponent>(Player, Plutus::DynamicBody);
        pbody->linearDamping = 0;
        pbody->gravityScale = 0;

        pbody->addBox({ 24, 18 }, { 12, 24 }, 1);
        pbody->addCircle({ 32, 24 }, 5, 1);
    }

    Logger::info("player init");

    auto ground = mScene->getEntityByName("Floor");
    if (ground) {
        auto body = ground.addComponent<Plutus::RigidBodyComponent>(ground, Plutus::StaticBody);
        auto trans = ground.getComponent<Plutus::TransformComponent>();

        body->addBox({ 0, 0 }, { (float)trans->w, (float)trans->h });
    }

    Logger::info("ground init");
    mSystemManager.start();
}

void GameScreen::update(float dt)
{

    float cscale = mWorldCamera.getScale();

    if (mInput->onKeyPressed("R")) {
        mWorldCamera.setScale(1);
    }

    auto cPos = mWorldCamera.getPosition();

    if (mInput->isCtrl) {

        if (mInput->onKeyDown("+")) {
            cscale += 0.05f;
            mWorldCamera.setScale(cscale);
        }

        if (mInput->onKeyDown("-")) {
            cscale -= 0.05f;
            mWorldCamera.setScale(cscale);
        }


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
    }
    else {
        auto trans = Player.getComponent<Plutus::TransformComponent>();

        auto tpos = Plutus::vec2f(round(trans->x), round(trans->y));

        cPos = tpos - mWorldCamera.getScaleScreen() * 0.5f;
    }
    mWorldCamera.setPosition(cPos);

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

    auto rbody = Player.getComponent<Plutus::RigidBodyComponent>();
    auto vel = rbody->mBody->GetLinearVelocity();
    // printf("vel: %0.3f %0.3f\n", vel.x, vel.y);

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
