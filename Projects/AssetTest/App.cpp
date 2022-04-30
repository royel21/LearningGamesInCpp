#include "App.h"
#include <unordered_map>

#include <Log/Logger.h>
// #include "Logger.h"

#include <Time/Timer.h>
#include <Input/Input.h>
#include <Utils/FileIO.h>
#include <Assets/Assets.h>
#include <ECS/Components.h>
#include <Systems/Systems.h>
#include <Graphics/DebugRenderer.h>
#include <Serialize/SceneSerializer.h>

#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val

namespace Plutus
{
    App::App(const char* name, int w, int h) {
        mName = name;
        mWidth = w;
        mHeight = h;
    }

    void App::Init() {
        mCamera.setScale(1.0f);
        // mCamera.setPosition(-640.00f, -340.00f);

        // mCamera.setViewPosition({ 0, 0 });

        mScene = mProject.scene.get();

        mFBuffer.init(mWidth, mHeight, true);

        mSystemManager.setProject(&mProject);
        auto render = mSystemManager.AddSystem<RenderSystem>(&mCamera);
        // render->setFrameBuffer(&mFBuffer);
        // mBatch = render->getSpritebath();

        mSystemManager.AddSystem<PhysicSystem>();
        mSystemManager.AddSystem<ScriptSystem>(&mCamera);

        mPlayer = mScene->createEntity("player");
        if (mPlayer) {
            AssetManager::get()->addAsset<Script>("player.lua", "assets/script/player.lua");
            // mCamera.setTarget(mPlayer);
            mPlayer.addComponent<TransformComponent>(10.0f, 50.0f, 32, 32);
            mPlayer.addComponent<SpriteComponent>("", ColorRGBA8{ 255,0,0,255 });
            mPlayer.addComponent<ScriptComponent>("player.lua");

            auto pbody = mPlayer.addComponent<RigidBodyComponent>();
            // pbody->mLinearDamping = 1;
            // pbody->mGravityScale = 0;

            pbody->addBox({ 0, 2 }, { 20, 20 }, 0.3f);
            pbody->setMaxVel(4);
            // pbody->addCircle({ 32, 24 }, 5, 1);
        }

        auto ground = mScene->createEntity("ground");

        if (ground) {
            ground.addComponent<TransformComponent>(5.0f, 10.0f, 1280, 10);
            ground.addComponent<SpriteComponent>("", ColorRGBA8{ 0,0,255,255 });

            auto body = ground.addComponent<Plutus::PhysicBodyComponent>();
            body->addBox({ 0, 0 }, { 1280, 10 }, 0.3f);
        }

        auto ground2 = mScene->createEntity("ground");

        if (ground2) {
            ground2.addComponent<TransformComponent>(100.0f, 25.0f, 200, 10);
            ground2.addComponent<SpriteComponent>("");

            auto body = ground2.addComponent<Plutus::PhysicBodyComponent>();
            body->addBox({ 0, 0 }, { 200, 10 });
        }

        auto ground3 = mScene->createEntity("ground3");

        if (ground3) {
            ground3.addComponent<TransformComponent>(200.0f, 60.0f, 200, 10);
            ground3.addComponent<SpriteComponent>("");

            auto body = ground3.addComponent<Plutus::PhysicBodyComponent>();
            body->addBox({ 0, 0 }, { 200, 10 });
        }

        mSystemManager.init();

        auto data = SceneSerializer(mScene);

        auto chunky = AssetManager::get()->addAsset<Sound>("Chunky", "assets/sounds/XYZ2.ogg", MUSIC);

        chunky->play();

    }

    float force = 0.2f;
    float force2 = 10.0f;
    Vec2f mouseLast;
    Vec2f camOrg;
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
            // mCamera.setPosition({ pos.x - 100, pos.y - 30 });
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
            Logger::info("camera Pos: %.2f %.2f", mouseLast.x, mouseLast.y);
        }

        if (Input::get()->onKeyPressed("MouseLeft"))
        {
            mouseLast = Input::get()->getMouseCoords();
            camOrg = mCamera.getPosition();
        }
        // move the camera
        if (Input::get()->isCtrl)
        {
            Vec2f offset;
            if (Input::get()->onKeyDown("MouseLeft"))
            {
                Vec2f result = Input::get()->getMouseCoords() - mouseLast;
                result /= mCamera.getScale();
                offset = camOrg - result;
                Logger::info("camera Pos: %.2f %.2f", result.x, result.y);
                mCamera.setPosition(offset);
            }

            auto scroll = Input::get()->getMouseWheel();
            if (scroll != 0)
            {
                auto mpos = mCamera.convertScreenToWold(Input::get()->getMouseCoords());

                auto newVal = mCamera.getScale() + (scroll > 0 ? 0.05f : -0.05f);
                mCamera.setScale(CHECKLIMIT(newVal, 0.20f, 6));
                auto newPos = mCamera.convertScreenToWold(Input::get()->getMouseCoords());

                auto offset = newPos - mpos;
                mCamera.setPosition(mCamera.getPosition() - offset);
                Logger::info("offset: %.2f %.2f", offset.x, offset.y);

            }
        }



        mSystemManager.update(dt);

    }

    void App::Draw() {
        // if (Input::get()->onKeyDown("MouseLeft")) {
        //     auto pos = mCamera.convertScreenToWold(Input::get()->getMouseCoords());

        //     auto ent = mScene->getEntity(mFBuffer.getEntId(pos));
        //     if (ent) {
        //         Logger::info("camera Pos: %s", ent.getName().c_str());
        //     }
        // }
        // mBatch->submit(mFBuffer.getTextureId(), Vec4f{ 0,0, mWidth, mHeight }, { 0, 0,1 ,1 }, {}, 0, false, true);
        // mBatch->finish();
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
