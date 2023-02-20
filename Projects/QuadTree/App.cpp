#include "App.h"
#include <Utils/Utils.h>

#include <Log/Logger.h>
#include <Time/Timer.h>

#include <Input/Input.h>

namespace Plutus
{
    constexpr uint32_t MAX_RECT = 1000;
    constexpr uint32_t scale = 5;
    constexpr uint32_t WIDTH = 1280 * scale;
    constexpr uint32_t HEIGHT = 720 * scale;

    bool LinearSearch = true;

    void App::run()
    {
        init();
        while (mWindow.isFinish()) {
            auto start = Time::micros();
            mLimiter.start();
            update();
            draw();

            mWindow.update();
            mLimiter.end();

            char title[128];
            std::snprintf(title, 128, "%s - FPS: %.2f elapse: %03.03f, count:%lu", LinearSearch ? "Linear" : "Quad", mLimiter.getFPS(), (Time::micros() - start) / 1000.0f, count);
            mWindow.setTitle(title);
        }
    }

    void App::init()
    {
        mQTrees.resize({ 0,0, WIDTH, HEIGHT });

        mWindow.init("Quad Tree Test", 1280, 720);
        mCamera.init(WIDTH, HEIGHT);

        mSpritebatch.init();
        mSpritebatch.setCamera(&mCamera);

        auto start = Time::micros();
        rects.reserve(MAX_RECT);
        mQTrees.reserve(MAX_RECT);
        for (size_t i = 0; i < MAX_RECT; i++)
        {
            float x = (float)Utils::getRandom(0, WIDTH - 50);
            float y = (float)Utils::getRandom(0, HEIGHT - 50);
            float w = (float)Utils::getRandom(20, 120);
            float h = (float)Utils::getRandom(20, 120);

            uint8_t r = (uint8_t)Utils::getRandom(0, 255);
            uint8_t g = (uint8_t)Utils::getRandom(0, 255);
            uint8_t b = (uint8_t)Utils::getRandom(0, 255);

            rects.emplace_back(Rect{ x, y, w, h }, ColorRGBA8(r, g, b));

            ColorRect crect(Rect{ x, y, w, h }, ColorRGBA8(r, g, b));

            mQTrees.insert(crect, crect.rect);
        }
        rect1 = { 0,0, 600, 600 };
        size_t totalItems = mQTrees.size();
        Logger::info("count: %zu %0.3fms", totalItems, (Time::micros() - start) / 1000.0f);

        mChildRects = mQTrees.getQTRects();
        Logger::info("rects: %zu", mChildRects.size());

        mDebugRender.init(&mCamera);

    }

    void App::update() {
        mCamera.update();
    }

    Vec2f pos;
    Vec2f pos2;
    bool isSelected = false;

    void App::draw()
    {
        glClearColor(0.6f, 0.7f, 1.0f, 1.0f);
        auto mpos = Input.getMouseCoords() * (float)scale;


        if (Input.onKeyPressed("Q")) {
            LinearSearch = !LinearSearch;
        }

        if (Input.onKeyPressed("MouseLeft") && rect1.contains(mpos)) {
            pos = mpos;
            pos2 = rect1.pos;
        }

        if (Input.onKeyDown("MouseLeft")) {
            auto newPos = mpos - pos;
            rect1.pos = pos2 + newPos;
        }
        auto scroll = Input.getMouseWheel();
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

        std::vector<ColorRect*> items;

        // auto start = Time::micros();
        // if (LinearSearch) {
        //     for (auto& r : rects) {
        //         if (rect1.overlaps(r.rect)) {
        //             items.push_back(&r);
        //         }
        //     }
        // }
        // else {
        //     items = mQTrees.query(rect1);
        // }
        items = mQTrees.query({ 0,0, WIDTH, HEIGHT });
        // Logger::info("count: %zu %0.3f", count, (Time::micros() - start) / 1000.0f);

        count = items.size();
        for (auto item : items) {
            mSpritebatch.submit(item->rect.getBox(), item->color);
        }

        // Rect v1(515, 259, 250, 250);

        ColorRGBA8 c(0, 0, 0, 10);
        // if (rect1.contains(v1)) {
        //     mSpritebatch.submit(v1.getBox());
        // }

        mSpritebatch.submit(rect1.getBox(), c);
        mSpritebatch.finish();

        for (auto rect : mChildRects) {
            mDebugRender.submitBox(rect->getBox());
        }
        mDebugRender.end();
        mDebugRender.render();

    }
}