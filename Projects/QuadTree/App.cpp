#include "App.h"
#include <Utils/Utils.h>

#include <Log/Logger.h>
#include <Time/Timer.h>

#include <Input/Input.h>

namespace Plutus
{
    constexpr uint32_t MAX_RECT = 1000000;
    constexpr uint32_t scale = 6;
    constexpr uint32_t WIDTH = 1280 * scale;
    constexpr uint32_t HEIGHT = 720 * scale;

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

    void App::init()
    {
        mQTrees.resize({ 0,0, WIDTH, HEIGHT });

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
            auto size = Rect{ x, y, w, h };

            ColorRect crect(size, ColorRGBA8(r, g, b));

            mQTrees.insert(crect, crect.rect);
        }
        rect1 = { 0,0, 600,600 };
        size_t totalItems = mQTrees.size();
        Logger::info("count: %zu", totalItems);
    }

    void App::update() {
        mCamera.update();
    }

    Vec2f pos;
    Vec2f pos2;
    bool isSelected = false;

    bool LinearSearch = true;

    void App::draw()
    {
        auto mpos = Input::get()->getMouseCoords() * (float)scale;
        auto start = Time::micros();
        glClearColor(0.6f, 0.7f, 1.0f, 1.0f);

        int count = 0;
        if (LinearSearch) {
            for (auto& r : rects) {
                if (rect1.overlaps(r.rect)) {
                    mSpritebatch.submit(r.rect.getBox(), r.color);
                    count++;
                }
            }
        }
        else {
            // for (auto& item : mQTrees.query(rect1)) {
            //     mSpritebatch.submit(item.rect.getBox(), item.color);
            //     count++;
            // }
        }


        Rect v1(515, 259, 250, 250);

        ColorRGBA8 c(0, 0, 0, 10);

        if (Input::get()->onKeyPressed("Q")) {
            LinearSearch = !LinearSearch;
        }

        if (Input::get()->onKeyPressed("MouseLeft") && rect1.contains(mpos)) {
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

        if (rect1.contains(v1)) {
            mSpritebatch.submit(v1.getBox());
        }
        mSpritebatch.submit(rect1.getBox(), c);
        mSpritebatch.finish();

        char title[128];
        std::snprintf(title, 128, "%s - elapse: %llu, FPS: %.2f count:%i / 1000000", LinearSearch ? "Linear" : "Quad", Time::micros() - start, mLimiter.getFPS(), count);
        mWindow.setTitle(title);
    }
}