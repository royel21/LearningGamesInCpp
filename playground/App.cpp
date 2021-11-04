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
    App::App(const char* name, int width, int height) : Core(name, width, height)
    {
        Input::get()->addEventListener(this);
    }

    void App::Setup()
    {
        // srand(time(NULL));   // Initialization, should only be called once.
        mDebug = DebugRender::get();
        mDebug->init(&mCamera);
        auto size = mCamera.getScaleScreen() - 5;
        size = vec2f(140.0f, size.y);
        shapes.push_back(new Circle2d{ 6, 200, 30 });
        // shapes.push_back(new Box2d{ 200, 100, 100, 100, 45 });
        shapes.push_back(new Box2d{ 360, 90, 100, 100 });
        // shapes.push_back(new Circle2d{ 100, 50, 50 });
        // shapes.push_back(new Circle2d{ 160, 170, 50 });
        // shapes.push_back(new Circle2d{ 250, 100, 50 });
        // shapes.push_back(new Circle2d{ 40, 80, 50 });
        float x = 40;
        float y = 40;
        for (size_t i = 0; i < 1; i++) {
            shapes.push_back(new Circle2d{ 30 + float(int(x * i) % 1100), 30 + float(int(y * i) % 650), 20 });
        }
        shapes.push_back(new Line2d{ 5, 5, 5, size.y }); // Left
        shapes.push_back(new Line2d{ 5, size.y, size.x, size.y }); // Top
        shapes.push_back(new Line2d{ size.x, size.y, size.x, 5 }); // Right
        shapes.push_back(new Line2d{ 5, 5, size.x, 5 }); // Bottom


        shapes.push_back(new Line2d{ 300, 300, 600, 600 });

        // shapes.push_back(new Line2d{ 0, 0, size.x - 300, size.y - 300 });

        // shapes.push_back(new Line2d{ 100, 40, 768, 900 });

        // for (float i = 0; i < 100; i++) {
        //     shapes.push_back(Circle2d{ float(rand() % 400), float(rand() % 400), float(40 + rand() % 100) });
        // }

    }
    bool isMouseDown = false;
    bool isMouseDownInCircle = false;
    bool isMouseDownInBox = false;

    vec2f initPos;
    vec2f pos;
    vec2f mpos;

    Circle2d c1 = { 640,384, 50 };
    Circle2d c2 = { 320,384, 25 };
    vec2f size = { 1280,768 };
    Line2d lineA = { 40, 30, 600, 30 };
    Line2d lineB = { 40, 29.999999f, 600, 250 };
    // Line2d lineCD = { size.x - 50, size.y - 30, size.x - 50, 50 };
    // Line2d lineDA = { 50, 50, size.x - 50, 50 };


    Box2d box(200, 200, 100, 100, 45);
    // Box2d box3(50, 50, 100, 100);
    Box2d box2(50, 200, 100, 100, 45);

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
            c1.pos = pos - dis;
        }

        for (size_t i = 0; i < shapes.size(); i++) {
            auto shapeA = shapes[i];
            if (shapeA->type & PCircle) {
                shapeA->pos.y -= 5;
            }
        }

        auto start = Timer::millis();
        // for (size_t y = 0; y < 8; y++)
        for (size_t i = 0; i < shapes.size(); i++) {
            MTV mtvA;
            auto shapeA = shapes[i];
            bool isCircleA = shapeA->type & PCircle;
            bool isBoxA = shapeA->type & PBox;
            bool isLineA = shapeA->type & PLine;

            for (size_t x = i + 1; x < shapes.size(); x++) {
                auto shapeB = shapes[x];
                bool isCircleB = shapeB->type & PCircle;
                bool isBoxB = shapeB->type & PBox;
                bool isLineB = shapeB->type & PLine;
                MTV mtv;

                if (isCircleA && isCircleB && Collider::isColliding((Circle2d*)shapeA, (Circle2d*)shapeB, &mtv)) {
                    // continue;
                }

                if (isCircleA && isLineB && Collider::isColliding((Circle2d*)shapeA, (Line2d*)shapeB, &mtv)) {
                    // continue;
                }

                if (isCircleA && isBoxB && Collider::isColliding((Circle2d*)shapeA, (Box2d*)shapeB, &mtv)) {
                    // continue;
                }

                // //Line Collision
                // // if (isLineA && isLineB) {
                // //     Collider::isColliding((Line2d*)shapeA, (Line2d*)shapeB, &manifold);
                // //     continue;
                // // }

                // if (isLineA && isCircleB && Collider::isColliding((Circle2d*)shapeB, (Line2d*)shapeA, &mtv)) {

                //     continue;
                // }

                // if (isLineA && isBoxB && Collider::isColliding((Box2d*)shapeA, (Line2d*)shapeB, &mtv)) {

                //     // printf("Line & Box\n");
                //     continue;
                // }

                // //Box Collision
                // if (isBoxA && isBoxB && Collider::isColliding((Box2d*)shapeA, (Box2d*)shapeB, &mtv)) {

                //     continue;
                // }

                // if (isBoxA && isCircleB && Collider::isColliding((Box2d*)shapeA, (Circle2d*)shapeB, &mtv)) {
                //     continue;
                // }

                // if (isBoxA && isLineB && Collider::isColliding((Box2d*)shapeA, (Line2d*)shapeB, &mtv)) {
                //     continue;
                // }
                if (isCircleB) {
                    auto half = mtv.axis * 0.5;
                    shapeB->pos += half;
                    mtvA.axis += half;
                }
                else {
                    mtvA.axis += mtv.axis;
                }
            }
            shapeA->pos += mtvA.axis;
            if (isCircleA) {
                DebugRender::get()->drawCircle(shapeA->pos, 5);

                printf("pos: %0.2f %0.2f \n", shapeA->pos.x, shapeA->pos.y);
            }
        }
        // printf("time: %llu\n", Timer::millis() - start);
    }

    void App::Draw()
    {


        for (auto shape : shapes) {
            switch (shape->type) {
            case PBox: {
                mDebug->drawBox(*(Box2d*)shape);
                break;
            }
            case PLine: {
                mDebug->drawLine(*(Line2d*)shape);
                break;
            }
            case PCircle: {
                mDebug->drawCircle(*(Circle2d*)shape);
                break;
            }
            }
        }

        // Points points = box.getVertices();



        // mDebug->drawCircle(c1);
        // mDebug->drawBox(box);
        // Points closest;
        // float dist = std::numeric_limits<float>::max();
        // for (auto& a : box.axes) {
        //     mDebug->drawLine(a, c1.pos, 0);
        //     // auto cp = (c1.pos - p).lengthSqrt();
        //     // if (cp < dist) {
        //     //     dist = cp;
        //     //     closest.push_back(p);
        //     // }
        // }

        // mDebug->drawCircle(pos, c1.radius);
        // auto unit = lineDist.unit();

        // mDebug->drawCircle(c1.pos, 5);
        // mDebug->drawCircle(lineA.pos - (unit * unit.dot(p1 - c1.pos)), 5);


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

        if (PUtils::PointInCircle(initPos, &c1)) {
            isMouseDownInCircle = true;
            pos = c1.pos;
        }
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