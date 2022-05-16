#include "App.h"
#include <Utils/Utils.h>

#include <Log/Logger.h>
#include <Time/Timer.h>

namespace Plutus
{
    constexpr uint32_t MAX_RECT = 100000;

    constexpr uint32_t WIDTH = 2560;
    constexpr uint32_t HEIGHT = 1440;


    void App::init()
    {
        mWindow.init("Quad Tree Test", 1280, 720);
        mCamera.init(WIDTH, HEIGHT);
        mShader.init();
        mSpritebatch.init();
        mSpritebatch.setShader(&mShader);
        mSpritebatch.setCamera(&mCamera);
        mSpriteBatch2.init();

        rects.reserve(MAX_RECT);
        for (size_t i = 0; i < MAX_RECT; i++)
        {
            float x = (float)Utils::getRandom(0, WIDTH);
            float y = (float)Utils::getRandom(0, HEIGHT);
            float w = (float)Utils::getRandom(20, 50);
            float h = (float)Utils::getRandom(20, 50);

            GLubyte r = (GLubyte)Utils::getRandom(0, 255);
            GLubyte g = (GLubyte)Utils::getRandom(0, 255);
            GLubyte b = (GLubyte)Utils::getRandom(0, 255);

            rects.emplace_back(Vec4f{ x, y, w, h }, PlutusEngine::ColorRGBA8(r, g, b, 255));
        }

    }

    void App::update() {
        mCamera.update();
    }

    void App::draw()
    {
        glClearColor(0.6f, 0.7f, 1.0f, 1.0f);
        mShader.enable();
        mShader.setUniformMat4("myCamera", mCamera.getCameraMatrix());
        mSpriteBatch2.begin();
        for (auto& r : rects) {
            mSpriteBatch2.submit(glm::vec4(r.rect.x, r.rect.y, r.rect.z, r.rect.w), r.color);
        }
        auto start = Time::micros();
        mSpriteBatch2.end();
        mSpriteBatch2.renderBatch();
        Logger::info("elapse: %llu, FPS: %.2f", Time::micros() - start, mLimiter.getFPS());
        // mSpritebatch.begin();
        // mSpritebatch.draw();
        // mSpritebatch.end();
        mShader.disable();
    }

    void App::run()
    {
        init();
        while (mWindow.isFinish()) {
            mLimiter.start();
            update();
            draw();

            mWindow.update();
            mLimiter.end();
        }
    }
}