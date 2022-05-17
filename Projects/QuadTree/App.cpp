#include "App.h"
#include <Utils/Utils.h>

#include <Log/Logger.h>
#include <Time/Timer.h>

#include <Input/Input.h>

namespace Plutus
{
    constexpr uint32_t MAX_RECT = 60000;

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

            uint8_t r = (uint8_t)Utils::getRandom(0, 255);
            uint8_t g = (uint8_t)Utils::getRandom(0, 255);
            uint8_t b = (uint8_t)Utils::getRandom(0, 255);

            rects.emplace_back(Rect{ x, y, w, h }, ColorRGBA8(r, g, b));
        }
        rect1 = { 0,0, 600,600 };
    }

    void App::update() {
        mCamera.update();
    }

    Vec2f pos;
    Vec2f pos2;
    bool isSelected = false;

    void App::draw()
    {
        auto mpos = Input::get()->getMouseCoords() * 2.0f;
        auto start = Time::micros();
        glClearColor(0.6f, 0.7f, 1.0f, 1.0f);

        int count = 0;
        for (auto& r : rects) {
            if (rect1.contain(r.rect)) {
                mSpritebatch.submit(r.rect.getBox(), r.color);
                count++;
            }
        }
        Rect v1(515, 259, 250, 250);

        ColorRGBA8 c(0, 0, 0, 10);

        if (Input::get()->onKeyPressed("MouseLeft") && rect1.interset(mpos)) {
            pos = mpos;
            pos2 = rect1.pos;
        }

        if (Input::get()->onKeyDown("MouseLeft")) {
            auto newPos = mpos - pos;
            rect1.pos = pos2 + newPos;
        }
        auto scroll = Input::get()->getMouseWheel();
        if (scroll != 0) {
            if (scroll > 0) {
                rect1.size += 20;
                rect1.pos -= 10;
            }
            else {
                rect1.size -= 20;
                rect1.pos += 10;
            }
        }

        if (rect1.contain(v1)) {
            mSpritebatch.submit(v1.getBox());
        }
        mSpritebatch.submit(rect1.getBox(), c);
        mSpritebatch.finish();

        char title[128];
        std::snprintf(title, 128, "elapse: %llu, FPS: %.2f count:%i", Time::micros() - start, mLimiter.getFPS(), count);
        mWindow.setTitle(title);
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