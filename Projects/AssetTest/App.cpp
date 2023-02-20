#include "App.h"
#include <unordered_map>

#include <Log/Logger.h>
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

#define NUM_PARTICLES 30000

namespace Plutus
{
    App::App(const char* name, int w, int h) {
        mName = name;
        mWidth = w;
        mHeight = h;
    }

    void App::Init() {
        printGLVersion();
        mSystemManager.setProject(&mProject);
        mWindow.setVSYNC(0);

        // AssetManager::get()->addAsset<Texture>("particles.png", "assets/textures/particle.png");
        AssetManager::get()->addAsset<Texture>("particles.png", "assets/textures/particle-textures/fire.png", 64, 64, GL_NEAREST);
        // AssetManager::get()->addAsset<Texture>("particles.png", "assets/textures/particle-textures/cosmic.png", 64, 64, GL_LINEAR);

        auto ent = mProject.scene->createEntity("particleEmiter");

        ent.addComponent<TransformComponent>(0.0f, 0.0f, 10, 10);
        auto particles = ent.addComponent<ParticleComponent>(NUM_PARTICLES);
        particles->color.setColor(255, 200, 200, 255);
        particles->addTexture("particles.png");
        particles->offset = { 0, -50 };

        auto sys = mSystemManager.AddSystem<ParticleSystem>(&mCamera);
        // sys->setBlend(false);
        mSystemManager.init();

        // for (int i = 0; i < NUM_PARTICLES; i++) {
        //     auto x = Utils::getRandom(-5, 5);
        //     auto y = Utils::getRandom(5, 20);
        //     particles->addParticle({ 640, 0 }, 100, { x, y }, 60.0f);
        // }
        // mLimiter.setFPS(120);
    }

    float force = 0.2f;
    float force2 = 10.0f;
    Vec2f mouseLast;
    Vec2f camOrg;


    void App::Update(float dt) {
        auto mpos = Input.getMouseCoords();
        auto particles = mProject.scene->getComponentFromName < ParticleComponent>("particleEmiter");
        auto cPos = mCamera.getPosition();

        if (Input.isCtrl) {

            if (Input.onKeyDown("Right")) {
                cPos.x += 5;
            }
            if (Input.onKeyDown("Left")) {
                cPos.x -= 5;
            }
            if (Input.onKeyDown("Up")) {
                cPos.y -= 5;
            }
            if (Input.onKeyDown("Down")) {
                cPos.y -= 5;
            }
            mCamera.setPosition(cPos);
        }

        if (Input.onKeyDown("MouseLeft")) {
            // if (timer.IntervalMillis(0)) {
            for (int i = 0; i < 1000; i++) {
                auto x = Utils::getRandom(-100, 100);
                auto y = Utils::getRandom(20, 100);
                particles->addParticle(mpos, 100, { x, y }, 0.50f);
            }
            // }
        }

        if (Input.onKeyPressed("MouseLeft"))
        {
            mouseLast = Input.getMouseCoords();
            camOrg = mCamera.getPosition();

        }
        // move the camera
        if (Input.isCtrl)
        {
            Vec2f offset;
            if (Input.onKeyDown("MouseLeft"))
            {
                Vec2f result = Input.getMouseCoords() - mouseLast;
                result /= mCamera.getScale();
                offset = camOrg - result;
                mCamera.setPosition(offset);
            }

            auto scroll = Input.getMouseWheel();
            if (scroll != 0)
            {
                auto newVal = mCamera.getScale() + (scroll > 0 ? 0.05f : -0.05f);
                mCamera.setScale(CHECKLIMIT(newVal, 0.20f, 6));
                auto newPos = mCamera.convertScreenToWold(Input.getMouseCoords());

                auto offset = newPos - mpos;
                mCamera.setPosition(mCamera.getPosition() - offset);

            }
        }
        setBackgoundColor(0.0f, 0.5f, 0.8f);

        auto start = Time::micros();
        mSystemManager.update(dt);
        // char title[128];
        // std::snprintf(title, 128, "FPS: %.2f elapse: %03.03f", mLimiter.getFPS(), (Time::micros() - start) / 1000.0f);
        // mWindow.setTitle(title);


        // auto start = Time::millis();
        Logger::info("count: %i - elapse: %llu", particles->count, 1000 / (Time::micros() - start));


        char title[128];
        std::snprintf(title, 128, "FPS: %.2f", mLimiter.getFPS());
        mWindow.setTitle(title);
    }

    void App::Draw() {
        setBackgoundColor(0.0f, 0.35f, 0.75f);
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
