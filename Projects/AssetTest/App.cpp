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

namespace Plutus
{
    void App::Setup() {

        mScene = CreateRef<Scene>();

        mSystemManager.setScene(mScene.get());
        mSystemManager.AddSystem<RenderSystem>(&mCamera);
        mSystemManager.AddSystem<PhysicSystem>();
        mSystemManager.AddSystem<ScriptSystem>(&mCamera);

        mPlayer = mScene->createEntity("player");
        if (mPlayer) {

            mPlayer.addComponent<TransformComponent>(10.0f, 50.0f, 64, 64);
            mPlayer.addComponent<SpriteComponent>("");
            mPlayer.addComponent<ScriptComponent>("assets/script/player.lua");

            auto pbody = mPlayer.addComponent<RigidBodyComponent>(mPlayer, DynamicBody);
            // pbody->mLinearDamping = 1;
            // pbody->mGravityScale = 0;

            pbody->addBox({ 0, 2 }, { 64, 62 });
            pbody->setMaxVel(4);
            // pbody->addCircle({ 32, 24 }, 5, 1);
        }

        auto ground = mScene->createEntity("ground");

        if (ground) {
            ground.addComponent<TransformComponent>(5.0f, 10.0f, 1280, 10);
            ground.addComponent<SpriteComponent>("");

            auto body = ground.addComponent<Plutus::RigidBodyComponent>(ground, Plutus::StaticBody);
            body->addBox({ 0, 0 }, { 1280, 10 }, 0.8f);
        }

        auto ground2 = mScene->createEntity("ground2");

        if (ground2) {
            ground2.addComponent<TransformComponent>(200.0f, 60.0f, 200, 10);
            ground2.addComponent<SpriteComponent>("");

            auto body = ground2.addComponent<Plutus::RigidBodyComponent>(ground2, Plutus::StaticBody);
            body->addBox({ 0, 0 }, { 200, 10 });
        }

        mSystemManager.init();
    }

    float force = 0.2f;
    float force2 = 10.0f;

    void App::Update(float dt) {
        mCamera.update();

        auto trans = mPlayer.getComponent<TransformComponent>();
        if (trans) {
            auto size = mCamera.getScaleScreen();
            mCamera.setPosition(vec2f{ trans->x - size.x * 0.5f, trans->y - size.y * 0.15f });
        }
        // }
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
