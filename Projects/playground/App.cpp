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
        // srand(time(NULL));   // Initialization, should only be called once.
        mDebug = DebugRender::get();
        mDebug->init(&mCamera);
        auto size = mCamera.getScaleScreen() - 5;
        size = Vec2f(size.x, size.y);
        shapes.push_back(new Circle2d{ 70, 100, 30 });
        shapes.push_back(new Circle2d{ 640, 150, 30 });
        // shapes.push_back(new Circle2d{ 70, 200, 30 });
        // shapes.push_back(new Circle2d{ 70, 250, 30 });
        // shapes.push_back(new Circle2d{ 70, 300, 30 });
        // shapes.push_back(new Box2d{ 200, 100, 100, 100, 45 });
        shapes.push_back(new Box2d{ 360, 90, 100, 100 });
        // shapes.push_back(new Circle2d{ 100, 50, 50 });
        // shapes.push_back(new Circle2d{ 160, 170, 50 });
        // shapes.push_back(new Circle2d{ 250, 100, 50 });
        // shapes.push_back(new Circle2d{ 40, 80, 50 });
        // float x = 40;
        // float y = 40;
        // for (size_t i = 0; i < 5; i++) {
        //     shapes.push_back(new Circle2d{ 50, 30 + float(int(y * i) % 650), 20 });
        // }
        shapes.push_back(new Line2d{ 5, 5, 5, size.y }); // Left
        shapes.push_back(new Line2d{ 5, size.y, size.x, size.y }); // Top
        shapes.push_back(new Line2d{ size.x, size.y, size.x, 5 }); // Right
        shapes.push_back(new Line2d{ 5, 5, size.x, 5 }); // Bottom


        shapes.push_back(new Line2d{ 80, 62, 600, 600 });

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
            if (Input::get()->onKeyDown("Up")) { shapes[controller]->pos.y += speed; }
            if (Input::get()->onKeyDown("Down")) { shapes[controller]->pos.y -= speed; }
            if (Input::get()->onKeyDown("Right")) { shapes[controller]->pos.x += speed; }
            if (Input::get()->onKeyDown("Left")) { shapes[controller]->pos.x -= speed; }
        }

        if (isMouseDownInBox) {
            auto dis = initPos - mpos;
            shapes[1]->pos = pos - dis;
        }

        if (isMouseDownInCircle) {
            auto dis = initPos - mpos;
        }

        for (size_t i = 0; i < shapes.size(); i++) {
            auto shapeA = shapes[i];
            if (shapeA->type & CircleShape) {
                shapeA->pos.y -= 5;
            }
        }

        for (size_t i = 0; i < shapes.size(); i++) {
            MTV mtvA;
            auto shapeA = shapes[i];
            bool isCircleA = shapeA->type & CircleShape;
            bool isBoxA = shapeA->type == BoxShape;
            bool isLineA = shapeA->type == EdgeShape;

            for (size_t x = i + 1; x < shapes.size(); x++) {
                auto shapeB = shapes[x];
                bool isCircleB = shapeB->type == CircleShape;
                bool isBoxB = shapeB->type == BoxShape;
                bool isLineB = shapeB->type == EdgeShape;
                MTV mtv;

                if (isCircleA && isLineB) {
                    Collider::isColliding((Circle2d*)shapeA, (Line2d*)shapeB, &mtv);
                }

                if (isCircleA && isCircleB) {
                    Collider::isColliding((Circle2d*)shapeA, (Circle2d*)shapeB, &mtv);
                }

                if (isCircleA && isBoxB) {
                    Collider::isColliding((Circle2d*)shapeA, (Box2d*)shapeB, &mtv);
                }

                if (isBoxA && isBoxB) {
                    Collider::isColliding((Box2d*)shapeA, (Box2d*)shapeB, &mtv);
                }

                if (isBoxA && isCircleB) {
                    Collider::isColliding((Box2d*)shapeA, (Circle2d*)shapeB, &mtv);
                }

                if (isBoxA && isLineB) {
                    Collider::isColliding((Box2d*)shapeA, (Line2d*)shapeB, &mtv);
                }

                if (isCircleB) {
                    auto half = mtv.axis * 0.5;
                    shapeA->pos += half;
                    shapeB->pos -= half;

                }
                else {
                    shapeA->pos += mtv.axis;
                }
            }
            if (isCircleA) {
                DebugRender::get()->drawCircle(shapeA->pos, 5);
            }
        }
    }

    void App::Draw()
    {
        for (auto shape : shapes) {
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
        for (auto shape : shapes) {
            delete shape;
        }
    }

    void App::onKeyDown(const std::string& key)
    {
        initPos = Input::get()->getMouseCoords();

        if (PUtils::PointInBox(initPos, (Box2d*)shapes[1])) {
            isMouseDownInBox = true;
            pos = shapes[1]->pos;
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