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
        // srand(time(NULL));   // Initialization, should only be called once.
        mDebug = DebugRender::get();
        mDebug->init(&mCamera);

        // size = Vec2f(size.x, size.y);
        // shapes.push_back(new Circle2d{ 70, 100, 30 });
        // shapes.push_back(new Circle2d{ 640, 150, 30 });
        // shapes.push_back(new Circle2d{ 640, 150, 30 });
        // shapes.push_back(new Circle2d{ 70, 200, 30 });
        // shapes.push_back(new Circle2d{ 70, 250, 30 });
        // shapes.push_back(new Circle2d{ 70, 300, 30 });
        // shapes.push_back(new Box2d{ 200, 100, 100, 100, 45 });
        // shapes.push_back(new Circle2d{ 100, 50, 50 });
        // shapes.push_back(new Circle2d{ 160, 170, 50 });
        // shapes.push_back(new Circle2d{ 250, 100, 50 });
        // shapes.push_back(new Circle2d{ 40, 80, 50 });

        mShapes.insert(new Circle2d{ 70, 100, 30 }, { 70, 100, 30, 30 });
        mShapes.insert(new Circle2d{ 640, 150, 30 }, { 640, 150, 30, 30 });
        mShapes.insert(new Circle2d{ 640, 150, 30 }, { 640, 150, 30, 30 });
        mShapes.back()->isStatic = true;
        mShapes.insert(new Box2d{ 360, 90, 100, 100 }, { 360, 90, 100, 100 });

        float x = 40;
        float y = 40;
        for (size_t i = 0; i < 0; i++) {
            float x = (float)Utils::getRandom(30, 1100);
            float y = (float)Utils::getRandom(30, 700);
            float size = (float)Utils::getRandom(20, 35);
            mShapes.insert(new Circle2d{ x, y, size }, { x, y, size, size });
        }
        // auto line = new Line2d{ 5, 5, 5, size.y };
        // line->isStatic = true;
        auto size = mCamera.getScaleScreen() - 5;
        mShapes.insert(new Line2d{ 5, 5, 5, size.y }, { 0, 0, 0, 0 });
        mShapes.back()->isStatic = true;

        mShapes.insert(new Line2d{ 5, size.y, size.x, size.y }, { 0, 0, 0, 0 });
        mShapes.back()->isStatic = true;

        mShapes.insert(new Line2d{ size.x, size.y, size.x, 5 }, { 0, 0, 0, 0 });
        mShapes.back()->isStatic = true;

        mShapes.insert(new Line2d{ 5, 5, size.x, 5 }, { 0, 0, 0, 0 });
        mShapes.back()->isStatic = true;

        mShapes.insert(new Line2d{ 80, 62, 600, 600 }, { 0, 0, 0, 0 });
        mShapes.back()->isStatic = true;

        // shapes.push_back(new Line2d{ 5, 5, 5, size.y }); // Left
        // shapes.back()->isStatic = true;
        // shapes.push_back(new Line2d{ 5, size.y, size.x, size.y }); // Top
        // shapes.back()->isStatic = true;
        // shapes.push_back(new Line2d{ size.x, size.y, size.x, 5 }); // Right
        // shapes.back()->isStatic = true;
        // shapes.push_back(new Line2d{ 5, 5, size.x, 5 }); // Bottom
        // shapes.back()->isStatic = true;


        // shapes.push_back(new Line2d{ 80, 62, 600, 600 });
        // shapes.back()->isStatic = true;

    }
    bool isMouseDown = false;
    bool isMouseDownInCircle = false;
    bool isMouseDownInBox = false;

    Vec2f initPos;
    Vec2f pos;
    Vec2f mpos;


    float speed = 5;

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
        auto cpos = mCamera.getPosition();
        if (Input::get()->isCtrl) {
            if (Input::get()->onKeyDown("Up")) { cpos.y += speed; }
            if (Input::get()->onKeyDown("Down")) { cpos.y -= speed; }
            if (Input::get()->onKeyDown("Right")) { cpos.x += speed; }
            if (Input::get()->onKeyDown("Left")) { cpos.x -= speed; }
            mCamera.setPosition(cpos);
        }
        else {
            if (Input::get()->onKeyDown("Up")) { mShapes[controller]->pos.y += speed; }
            if (Input::get()->onKeyDown("Down")) { mShapes[controller]->pos.y -= speed; }
            if (Input::get()->onKeyDown("Right")) { mShapes[controller]->pos.x += speed; }
            if (Input::get()->onKeyDown("Left")) { mShapes[controller]->pos.x -= speed; }
        }

        if (isMouseDownInBox) {
            auto dis = initPos - mpos;
            mShapes[1]->pos = pos - dis;
        }

        if (isMouseDownInCircle) {
            auto dis = initPos - mpos;
        }

        for (size_t i = 0; i < mShapes.size(); i++) {
            auto shapeA = mShapes[i];
            if (shapeA->type == CircleShape && !shapeA->isStatic) {
                shapeA->pos.y -= 5;
            }
        }

        Time::Log("collider: ");
        for (size_t i = 0; i < mShapes.size(); i++) {
            MTV mtvA;
            auto shapeA = mShapes[i];
            if (shapeA->isStatic) continue;
            bool isCircleA = shapeA->type & CircleShape;
            bool isBoxA = shapeA->type == BoxShape;
            bool isLineA = shapeA->type == EdgeShape;

            for (size_t x = i + 1; x < mShapes.size(); x++) {
                auto shapeB = mShapes[x];
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
            if (isCircleA) {
                DebugRender::get()->drawCircle(shapeA->pos, 5);
            }
        }
        Time::LogEnd("collider: ");
    }

    void App::Draw()
    {
        for (auto shape : mShapes) {
            switch (shape->type) {
            case BoxShape: {
                mDebug->drawBox(*(Box2d*)shape);
                break;
            }
            case EdgeShape: {
                mDebug->drawLine(*(Line2d*)shape);
                break;
            }
            case CircleShape: {
                mDebug->drawCircle(*(Circle2d*)shape);
                break;
            }
            }
        }

        mDebug->render();
        mDebug->end();
    }

    void App::Exit()
    {
        for (auto shape : mShapes) {
            delete shape;
        }
    }

    void App::onKeyDown(const std::string& key)
    {
        initPos = Input::get()->getMouseCoords();

        if (PUtils::PointInBox(initPos, (Box2d*)mShapes[1])) {
            isMouseDownInBox = true;
            pos = mShapes[1]->pos;
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