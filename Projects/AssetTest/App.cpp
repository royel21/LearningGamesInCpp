#include "App.h"
#include <Assets/Assets.h>

#include <Log/Logger.h>
#include <Utils/FileIO.h>
// #include "Logger.h"
#include <Time/Timer.h>

#include <unordered_map>
#include <Graphics/DebugRenderer.h>

#include <ECS/Components.h>

#include <Systems/Systems.h>

#include <Serialize/SceneSerializer.h>

namespace Plutus
{
    App::App(const char* name, int w, int h) {
        mName = name;
        mWidth = w;
        mHeight = h;
    }

    void App::Init() {

        mCamera.setScale(1.2f);
        mCamera.init(1280, 720, false);
        // mCamera.setPosition(-640.00f, -340.00f);

        // mCamera.setViewPosition({ 0, 0 });

        mScene = CreateRef<Scene>();

        mSystemManager.setScene(mScene.get());
        mSystemManager.AddSystem<RenderSystem>(&mCamera);
        mSystemManager.AddSystem<PhysicSystem>();
        mSystemManager.AddSystem<ScriptSystem>(&mCamera);

        mPlayer = mScene->createEntity("player");
        if (mPlayer) {
            // mCamera.setTarget(mPlayer);
            mPlayer.addComponent<TransformComponent>(10.0f, 50.0f, 32, 32);
            mPlayer.addComponent<SpriteComponent>("");
            mPlayer.addComponent<ScriptComponent>("assets/script/player.lua");

            auto pbody = mPlayer.addComponent<RigidBodyComponent>(mPlayer);
            // pbody->mLinearDamping = 1;
            // pbody->mGravityScale = 0;

            pbody->addBox({ 0, 2 }, { 20, 20 });
            pbody->setMaxVel(4);
            // pbody->addCircle({ 32, 24 }, 5, 1);
        }

        auto ground = mScene->createEntity("ground");

        if (ground) {
            ground.addComponent<TransformComponent>(5.0f, 10.0f, 1280, 10);
            ground.addComponent<SpriteComponent>("");

            auto body = ground.addComponent<Plutus::PhysicBodyComponent>(ground);
            body->addBox({ 0, 0 }, { 1280, 10 }, 0.8f);
        }

        auto ground2 = mScene->createEntity("ground2");

        if (ground2) {
            ground2.addComponent<TransformComponent>(200.0f, 60.0f, 200, 10);
            ground2.addComponent<SpriteComponent>("");

            auto body = ground2.addComponent<Plutus::PhysicBodyComponent>(ground2);
            body->addBox({ 0, 0 }, { 200, 10 });
        }

        auto ground3 = mScene->createEntity("ground2");

        if (ground3) {
            ground3.addComponent<TransformComponent>(100.0f, 25.0f, 200, 10);
            ground3.addComponent<SpriteComponent>("");

            auto body = ground3.addComponent<Plutus::RigidBodyComponent>(ground3, Plutus::StaticBody);
            body->addBox({ 0, 0 }, { 200, 10 });
        }

        mSystemManager.init();

        auto data = SceneSerializer(mScene.get());

        Utils::saveFile("./scene.json", data.c_str());

    }

    float force = 0.2f;
    float force2 = 10.0f;
    vec2f mouseLast;
    vec2f camOrg;
    void App::Update(float dt) {
        auto cPos = mCamera.getPosition();

        auto mInput = Input::get();

        if (mInput->isCtrl) {

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
            mCamera.setPosition(cPos);
        }
        else {
            auto trans = mPlayer.getComponent<TransformComponent>();
            // if (trans) {
            //     auto size = mCamera.getScaleScreen();
            // // // auto vec = mCamera.convertScreenToWold(pos);

            // auto pos = trans->getPosition();
            // mCamera.setPosition({ pos.x - 10, pos.y - 10 });
            // pos = mCamera.getPosition();
            // Logger::info("camera Pos: %.2f %.2f", pos.x, pos.y);
            // }
        }
        // auto pos = mCamera.getPosition();
        // auto woldOrg = mCamera.convertScreenToWold({ 0,0 });
        // Logger::info("camera Pos: %.2f %.2f world: %.2f %.2f", pos.x, pos.y, woldOrg.x, woldOrg.y);
        // mCamera.setPosition(-woldOrg);
        if (Input::get()->onKeyPressed("MouseLeft")) {
            // auto pos = Input::get()->getMouseCoords();
            // auto worldPos = mCamera.convertScreenToWold(pos);
            // Logger::info("camera Pos: %.2f %.2f world: %.2f %.2f", pos.x, pos.y, worldPos.x, worldPos.y);
            mouseLast = mCamera.convertScreenToWold(Input::get()->getMouseCoords());
            camOrg = mCamera.getPosition();
            Logger::info("camera Pos: %.2f %.2f", mouseLast.x, mouseLast.y);
        }
        camOrg = mCamera.getPosition();
        Logger::info("camera Pos: %.2f %.2f", camOrg.x, camOrg.y);

        if (Input::get()->onKeyDown("MouseLeft")) {
            // auto result = Input::get()->getMouseCoords() - mouseLast;
            // auto newPos = camOrg - result;
            // Logger::info("camera Pos: %.2f %.2f", result.x, result.y);
            // mCamera.setPosition({ 217.00f, 6.00f });
        }
        // mCamera.setPosition({ -217.00f, -6.00f });

        mSystemManager.update(dt);

    }

    void App::Draw() {

        // mRenderer.submit(tex->mTexId, rect1, tex->getUV(0));
        // mRenderer.submit(tex->mTexId, rect2, tex->getUV(0));
        // mRenderer.finish();

        // mRenderer.submit("Zoika", "Testing Font", 0, 256.0f);
        // mRenderer.submit("Arial", "Testing Font", 0, 192.0f);
        // mRenderer.finish(BATCH_TEXT);


        // mDebug->drawGrid();
        // mDebug->drawBox(rect1);
        // mDebug->drawBox(rect2);

        // mDebug->drawCircle({ Circle2d{100, 150, 50} });
        // mDebug->end();
        // mDebug->render(2);
    }

    void App::Resize(int w, int h) {
        glViewport(0, 0, w, h);
        mCamera.setWindowSize({ w, h });
    }

    void App::Exit() {
        mSystemManager.cleanup();
        AssetManager::get()->destroy();
    }
} // namespace Plutus
