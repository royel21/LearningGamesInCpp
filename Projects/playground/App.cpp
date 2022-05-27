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

        Input::get()->addEventListener(this);
    }

    void App::Init()
    {
        mShapes.resize({ 0, 0, (float)mWidth, (float)mHeight });
        // setAlwaysOnTop(true);
        mDebug = DebugRender::get();
        mDebug->init(&mCamera);

        mShapes.insert<Circle2d>(70.0f, 100.0f, 30.0f);
        mShapes.insert<Circle2d>(640.0f, 150.0f, 30.0f);
        mShapes.insert<Circle2d>(640.0f, 100.0f, 30.0f);
        mShapes.back().ref->isStatic = true;
        mShapes.insert<Box2d>(360.0f, 90.0f, 100.0f, 100.0f);
        mShapes.back().ref->isStatic = true;

        float x = 40;
        float y = 40;
        for (size_t i = 0; i < 300; i++) {
            float x = (float)Utils::getRandom(30, 1100);
            float y = (float)Utils::getRandom(30, 700);
            float size = (float)Utils::getRandom(20, 25);
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
        if (Input::get()->onKeyDown("+")) {
            cscale += 0.05f;
            mCamera.setScale(cscale);
        }
        if (Input::get()->onKeyDown("-")) {
            cscale -= 0.05f;
            mCamera.setScale(cscale);
        }
        auto shape = mShapes[controller].ref;

        auto cpos = mCamera.getPosition();

        if (Input::get()->isCtrl) {
            if (Input::get()->onKeyDown("Up")) { cpos.y += speed; }
            if (Input::get()->onKeyDown("Down")) { cpos.y -= speed; }
            if (Input::get()->onKeyDown("Right")) { cpos.x += speed; }
            if (Input::get()->onKeyDown("Left")) { cpos.x -= speed; }
            mCamera.setPosition(cpos);
        }
        else {
            if (Input::get()->onKeyDown("Up")) { shape->pos.y += speed; }
            if (Input::get()->onKeyDown("Down")) { shape->pos.y -= speed; }
            if (Input::get()->onKeyDown("Right")) { shape->pos.x += speed; }
            if (Input::get()->onKeyDown("Left")) { shape->pos.x -= speed; }
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
        for (size_t i = 0; i < mShapes.size(); i++) {
            auto shapeA = mShapes[i].ref;
            if (shapeA->isStatic) continue;
            bool isCircleA = shapeA->type & CircleShape;
            bool isBoxA = shapeA->type == BoxShape;
            bool isLineA = shapeA->type == EdgeShape;

            auto items = mShapes.query(shapeA->getRect());

            for (auto& item : items) {
                if (i == item->index) continue;
                auto shapeB = mShapes[item->index].ref;
                // for (size_t x = 0; x < mShapes.size(); x++) {
                //     auto shapeB = mShapes[x].ref;
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
        Logger::info("collider: %llu", Time::micros() - start);
    }

    void App::drawQuadrant(QuadTree* qt) {

        for (auto& ch : qt->mChilds) {
            if (ch) drawQuadrant(ch.get());
        }

        auto box = Box2d{ qt->mRect.pos, qt->mRect.size };
        mDebug->submitBox(&box);
    }

    void App::Draw()
    {
        for (auto& item : mShapes) {
            auto shape = item.ref;
            switch (shape->type) {
            case BoxShape: {
                mDebug->submitBox((Box2d*)shape);
                break;
            }
            case EdgeShape: {
                mDebug->submitLine((Line2d*)shape);
                break;
            }
            case CircleShape: {
                DebugRender::get()->submitCircle(shape->pos, 5);
                mDebug->submitCircle((Circle2d*)shape);
                break;
            }
            }
            // if (shape->type == CircleShape) {
            //     auto rect = shape->getRect();
            //     auto box = Box2d{ rect.pos, rect.size };
            //     mDebug->submitBox(&box);
            // }
        }

        drawQuadrant(&mShapes.root);

        mDebug->render();
        mDebug->end();
    }

    void App::Exit()
    {
        mShapes.clear();
    }

    void App::onKeyDown(const std::string& key)
    {
        initPos = Input::get()->getMouseCoords();

        if (PUtils::PointInBox(initPos, (Box2d*)mShapes[1].ref)) {
            isMouseDownInBox = true;
            pos = mShapes[1].ref->pos;
        }

        // if (PUtils::PointInCircle(initPos, &c1)) {
        //     isMouseDownInCircle = true;
        //     pos = c1.pos;
        // }
    }

    void App::onKeyUp(const std::string& key)
    {
        isMouseDownInCircle = isMouseDownInBox = false;
    }

    void App::onMouseMove(float x, float y)
    {
        mpos = Input::get()->getMouseCoords();
    }
}