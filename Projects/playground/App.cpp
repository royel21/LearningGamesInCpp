#include "App.h"
#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>

#include <Math/Vectors.h>
#include <Physics/Collider.h>
#include <Physics/PUtils.h>
#include <Physics/Shapes.h>

#include <limits>
#include <algorithm>

#include <time.h>
#include <stdlib.h>

#include <Math/PMath.h>
#include <Time/Timer.h>
#include <Utils/Utils.h>
#include <GLFW/glfw3.h>

namespace Plutus
{
    template<typename T>
    T* castShape(Shape* shape) {
        return static_cast<T*>(shape);
    }

    App::App(const char* name, int width, int height)
    {
        mName = name;
        mWidth = width;
        mHeight = height;

        Input.addEventListener(this);
    }

    void App::Init()
    {
        mCamera.setScale(.5f);
        auto scSize = mCamera.getScaleScreen();
        mShapes.resize({ 0, 0, scSize.x, scSize.y });
        // setAlwaysOnTop(true);
        mDebug = DebugRender::get();
        mDebug->init(&mCamera);
        mWindow.setVSYNC(1);

        mShapes.insert<Circle2d>(70.0f, 100.0f, 30.0f);
        // mShapes.insert<Circle2d>(640.0f, 150.0f, 30.0f);
        // mShapes.insert<Circle2d>(640.0f, 100.0f, 30.0f);
        // mShapes.back().ref->isStatic = true;
        // mShapes.insert<Box2d>(360.0f, 90.0f, 100.0f, 100.0f);
        // mShapes.back().ref->isStatic = true;

        float x = 40;
        float y = 40;
        for (size_t i = 0; i < 2000; i++) {
            float x = (float)Utils::getRandom(30, int(scSize.x - 80));
            float y = (float)Utils::getRandom(30, int(scSize.y - 80));
            float size = (float)Utils::getRandom(10, 25);
            mShapes.insert<Circle2d>(x, y, size);
        }

        auto size = mCamera.getScaleScreen() - 5;
        mShapes.insert<Line2d>(5.0f, 5.0f, 5.0f, size.y);
        mShapes.back().ref->isStatic = true;

        mShapes.insert<Line2d>(5.0f, size.y, size.x, size.y);
        mShapes.back().ref->isStatic = true;

        mShapes.insert<Line2d>(size.x, size.y, size.x, 5.0f);
        mShapes.back().ref->isStatic = true;

        mShapes.insert<Line2d>(5.0f, 5.0f, size.x, 5.0f);
        mShapes.back().ref->isStatic = true;

        mShapes.insert<Line2d>(80.0f, 62.0f, 600.0f, 600.0f);
        mShapes.back().ref->isStatic = true;
    }
    bool isMouseDown = false;
    bool isMouseDownInCircle = false;
    bool isMouseDownInBox = false;

    Vec2f initPos;
    Vec2f pos;
    Vec2f mpos;


    float speed = 10;

    int controller = 0;

    void App::Update(float dt)
    {
        float cscale = mCamera.getScale();
        if (Input.onKeyDown("+")) {
            cscale += 0.05f;
            mCamera.setScale(cscale);
        }
        if (Input.onKeyDown("-")) {
            cscale -= 0.05f;
            mCamera.setScale(cscale);
        }
        auto shape = mShapes[controller].ref;

        auto cpos = mCamera.getPosition();

        if (Input.isCtrl) {
            if (Input.onKeyDown("Up")) { cpos.y += speed; }
            if (Input.onKeyDown("Down")) { cpos.y -= speed; }
            if (Input.onKeyDown("Right")) { cpos.x += speed; }
            if (Input.onKeyDown("Left")) { cpos.x -= speed; }
            mCamera.setPosition(cpos);
        }
        else {
            if (Input.onKeyDown("Up")) { shape->pos.y += speed; }
            if (Input.onKeyDown("Down")) { shape->pos.y -= speed; }
            if (Input.onKeyDown("Right")) { shape->pos.x += speed; }
            if (Input.onKeyDown("Left")) { shape->pos.x -= speed; }
        }


        // if (isMouseDownInBox) {
        //     auto dis = initPos - mpos;
        //     mShapes[1]->pos = pos - dis;
        // }

        if (isMouseDownInCircle) {
            auto dis = initPos - mpos;
        }

        for (auto& item : mShapes) {
            auto shape = item.ref;

            if (!shape->isStatic) {
                // shape->pos.y -= 2;
                auto qRef = mShapes.getQuadItemListRef(shape->getRect());
                if (qRef != item.qRef) {
                    // printf("dif \n", item.qIndex, item.index);

                    qRef->mItems.push_back(item.qRef->mItems[item.qIndex]);

                    if (item.qRef->size()) {
                        item.qRef->mItems[item.qIndex] = item.qRef->mItems.back();
                        mShapes[item.qRef->mItems[item.qIndex].second].qIndex = item.qIndex;
                    }

                    item.qRef->mItems.pop_back();

                    item.qRef = qRef;
                    item.qIndex = qRef->mItems.size() - 1;
                }
            }
        }

        auto start = Time::micros();
        count = 0;

        for (size_t i = 0; i < mShapes.size(); i++) {
            auto shapeA = mShapes[i].ref;
            shapeA->color = { 255,255,255,255 };

            if (shapeA->isStatic) continue;

            bool isCircleA = shapeA->type & CircleShape;
            bool isBoxA = shapeA->type == BoxShape;
            bool isLineA = shapeA->type == EdgeShape;

            auto items = mShapes.query(shapeA->getRect());
            for (auto& item : items) {
                if (i == item) continue;
                auto shapeB = mShapes[item].ref;

                // for (size_t x = i + 1; x < mShapes.size(); x++) {
                //     auto shapeB = mShapes[x].ref;
                count++;
                bool isCircleB = shapeB->type == CircleShape;
                bool isBoxB = shapeB->type == BoxShape;
                bool isLineB = shapeB->type == EdgeShape;
                bool collided = false;
                MTV mtv;
                if (isCircleA && isLineB) {
                    collided = Collider::isColliding((Circle2d*)shapeA, (Line2d*)shapeB, &mtv);
                }

                if (isCircleA && isCircleB) {
                    collided = Collider::isColliding((Circle2d*)shapeA, (Circle2d*)shapeB, &mtv);
                }

                if (isCircleA && isBoxB) {
                    collided = Collider::isColliding((Circle2d*)shapeA, (Box2d*)shapeB, &mtv);
                }

                if (isBoxA && isBoxB) {
                    collided = Collider::isColliding((Box2d*)shapeA, (Box2d*)shapeB, &mtv);
                }

                if (isBoxA && isCircleB) {
                    collided = Collider::isColliding((Box2d*)shapeA, (Circle2d*)shapeB, &mtv);
                }

                if (isBoxA && isLineB) {
                    collided = Collider::isColliding((Box2d*)shapeA, (Line2d*)shapeB, &mtv);
                }
                if (collided) {
                    if (shapeA->pos.y < 29.5f) {
                        shapeA->pos.y = 30.0f;
                    }
                    if (!shapeB->isStatic) {
                        auto half = mtv.axis * 0.5;
                        shapeA->pos += half;
                        shapeB->pos -= half;
                    }
                    else {
                        shapeA->pos += mtv.axis;
                    }
                }
            }
        }
        auto time = Time::micros() - start;
        // Logger::info("collider: %llu - count: %zu", time, count);
        {
            auto items = mShapes.query(shape->getRect());
            for (auto i : items) {
                mShapes[i].ref->color = { 0,0,255,255 };
            }
        }
    }

    void App::drawQuadrant(QuadTree* qt) {

        for (auto& ch : qt->mChilds) {
            if (ch) drawQuadrant(ch.get());
        }

        auto box = Box2d{ qt->mRect.pos, qt->mRect.size };
        mDebug->submitBox(&box);
        count++;
    }

    void App::Draw()
    {
        count = 0;
        auto start = Time::micros();
        for (auto& item : mShapes) {
            count++;
            auto shape = item.ref;
            switch (shape->type) {
            case BoxShape: {
                mDebug->submitBox((Box2d*)shape, shape->color);
                break;
            }
            case EdgeShape: {
                mDebug->submitLine((Line2d*)shape, shape->color);
                break;
            }
            case CircleShape: {
                mDebug->submitCircle((Circle2d*)shape, shape->color);
                break;
            }
            }
        }

        drawQuadrant(&mShapes.root);
        Logger::info("collider: %llu C:%lu", Time::micros() - start, count);
        mDebug->end();
        mDebug->render();

        char title[128];
        std::snprintf(title, 128, "FPS: %.2f", getFPS());
        mWindow.setTitle(title);
    }

    void App::Exit()
    {
        mShapes.clear();
    }

    void App::onKeyDown(const std::string& key)
    {
        initPos = Input.getMouseCoords();

        if (PUtils::PointInBox(initPos, (Box2d*)mShapes[1].ref)) {
            isMouseDownInBox = true;
            pos = mShapes[1].ref->pos;
        }
    }

    void App::onKeyUp(const std::string& key)
    {
        isMouseDownInCircle = isMouseDownInBox = false;
    }

    void App::onMouseMove(float x, float y)
    {
        mpos = Input.getMouseCoords();
    }
}