#include "App.h"
#include <unordered_map>

#include <Log/Logger.h>
// #include "Logger.h"

#include <Time/Timer.h>
#include <Input/Input.h>
#include <Utils/FileIO.h>
#include <Utils/Utils.h>
#include <Assets/Assets.h>
#include <ECS/Components.h>
#include <Systems/Systems.h>

#include "ParticleComponent.h"
#include "ParticleSystem.h"


#define CHECKLIMIT(val, min, max) val<min ? min : val> max ? max : val

namespace Plutus
{
    App::App(const char* name, int w, int h) {
        mName = name;
        mWidth = w;
        mHeight = h;
    }

    void App::Init() {
        mSystemManager.setProject(&mProject);

        AssetManager::get()->addAsset<Texture>("particles.png", "assets/textures/particle-textures/particleAtlas.png", 32, 32, GL_LINEAR, GL_LINEAR);

        auto ent = mProject.scene->createEntity("particleEmiter");

        ent.addComponent<TransformComponent>(0.0f, 0.0f, 10, 10);
        auto particles = ent.addComponent<ParticleComponent>(100);
        // particles->color.setColor(255, 255, 255, 255);
        particles->addTexture("particles.png");

        mSystemManager.AddSystem<ParticleSystem>(&mCamera);

        mSystemManager.init();
        Logger::info("init");
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
            auto pos = mCamera.convertScreenToWold(Input::get()->getMouseCoords());
            // auto worldPos = mCamera.convertScreenToWold(pos);
            // Logger::info("camera Pos: %.2f %.2f world: %.2f %.2f", pos.x, pos.y, worldPos.x, worldPos.y);
            // mouseLast = mCamera.convertScreenToWold(Input::get()->getMouseCoords());
            // Logger::info("camera Pos: %.2f %.2f", mouseLast.x, mouseLast.y);

            auto ent = mProject.scene->getEntityByName("particleEmiter");
            auto particles = ent.getComponent<ParticleComponent>();

            for (int i = 0; i < 10; i++) {
                auto x = Utils::getRandom(-5, 5);
                auto y = Utils::getRandom(0, 50);
                particles->addParticle(pos, 100, { x, y }, 4.0f);
                // Logger::info("num: %i %i", x, y);
            }
            Logger::warn("Next");
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
                // Logger::info("camera Pos: %.2f %.2f", result.x, result.y);
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
                // Logger::info("offset: %.2f %.2f", offset.x, offset.y);

            }
        }
        // setBackgoundColor(0.0f, 0.5f, 0.8f);

        auto start = Timer::micros();
        mSystemManager.update(dt);
        Logger::info("elapse: %llu", Timer::micros() - start);

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
