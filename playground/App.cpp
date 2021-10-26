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
        shapes.push_back(new Circle2d{ 640, 600, 50 });
        // shapes.push_back(new Circle2d{ 400, 700, 50 });
        // shapes.push_back(new Circle2d{ 100, 50, 50 });
        // shapes.push_back(new Circle2d{ 160, 170, 50 });
        // shapes.push_back(new Circle2d{ 250, 100, 50 });
        // shapes.push_back(new Circle2d{ 40, 80, 50 });
        shapes.push_back(new Line2d{ 5, 5, 5, size.y });
        shapes.push_back(new Line2d{ 5, size.y, size.x, size.y });
        shapes.push_back(new Line2d{ size.x, size.y, size.x, 5 });
        shapes.push_back(new Line2d{ 5, 5, size.x, 5 });

        shapes.push_back(new Line2d{ 0, 0, size.x - 300, size.y - 300 });

        shapes.push_back(new Line2d{ 100, 40, 768, 900 });

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
    Line2d line = { 600,300, 600, 750, 90 };
    Line2d line2 = { 400,200, 400, 600, 45 };

    vec2f size = { 1280,768 };
    // Line2d lineAB = { 5, 1, 5, size.y };
    // Line2d lineBC = { 1, size.y - 5, size.x, size.y - 5 };
    // Line2d lineCD = { size.x - 50, size.y - 30, size.x - 50, 50 };
    // Line2d lineDA = { 50, 50, size.x - 50, 50 };


    Box2d box(50, 50, 100, 100, 45);
    // Box2d box3(50, 50, 100, 100);
    Box2d box2(50, 200, 100, 100);

    float speed = 5;
    void App::Update(float dt)
    {
        if (Input::get()->onKeyDown("Up")) { shapes[0]->pos.y += speed; }
        if (Input::get()->onKeyDown("Down")) { shapes[0]->pos.y -= speed; }
        if (Input::get()->onKeyDown("Right")) { shapes[0]->pos.x += speed; }
        if (Input::get()->onKeyDown("Left")) { shapes[0]->pos.x -= speed; }

        if (isMouseDownInBox) {
            auto dis = initPos - mpos;
            box.pos = pos - dis;
        }
        if (isMouseDownInCircle) {
            auto dis = initPos - mpos;
            shapes[0]->pos = pos - dis;
        }
        for (size_t i = 0; i < shapes.size(); i++) {
            auto shapeA = shapes[i];
            if (shapeA->type & PCircle) {
                shapeA->pos.y -= 8;
            }
        }

        for (size_t i = 0; i < shapes.size(); i++) {
            auto shapeA = shapes[i];
            bool isCircleA = shapeA->type & PCircle;
            bool isBoxA = shapeA->type & PBox;
            bool isLineA = shapeA->type & PLine;

            Manifold manifold;

            for (size_t x = i + 1; x < shapes.size(); x++) {
                auto shapeB = shapes[x];
                bool isCircleB = shapeB->type & PCircle;
                bool isBoxB = shapeB->type & PBox;
                bool isLineB = shapeB->type & PLine;

                if (isCircleA && isCircleB) {
                    Collider::isColliding((Circle2d*)shapeA, (Circle2d*)shapeB, &manifold);
                    continue;
                }

                if (isCircleA && isLineB) {
                    Collider::isColliding((Circle2d*)shapeA, (Line2d*)shapeB, &manifold);
                    continue;
                }

                if (isCircleA && isBoxB) {
                    Collider::isColliding((Box2d*)shapeB, (Circle2d*)shapeA, &manifold);
                    continue;
                }

                //Line Collision
                if (isLineA && isLineB) {
                    Collider::isColliding((Line2d*)shapeA, (Line2d*)shapeB, &manifold);
                    continue;
                }

                if (isLineA && isCircleB) {
                    Collider::isColliding((Circle2d*)shapeB, (Line2d*)shapeA, &manifold);
                    continue;
                }

                if (isLineA && isBoxB) {
                    Collider::isColliding((Box2d*)shapeA, (Line2d*)shapeB, &manifold);
                    continue;
                }

                //Box Collision
                if (isBoxA && isBoxB) {
                    Collider::isColliding((Box2d*)shapeA, (Box2d*)shapeB, &manifold);
                    continue;
                }

                if (isBoxA && isCircleB) {
                    Collider::isColliding((Box2d*)shapeA, (Circle2d*)shapeB, &manifold);
                    continue;
                }

                if (isBoxA && isLineB) {
                    Collider::isColliding((Box2d*)shapeA, (Line2d*)shapeB, &manifold);
                    continue;
                }
            }
            if (isCircleA) {
                vec2f forces;
                for (auto& f : manifold.forces) {
                    forces += f;
                }
                shapeA->pos += forces;
            }
        }
    }


    void App::Draw()
    {

        // if (Collider::isColliding(c1, c2, &manifold)) {
        //     printf("Circle & Circle\n");
        // }

        // if (Collider::isColliding(c1, lineDA, &manifold) || Collider::isColliding(c1, lineCD, &manifold)) {
        //     printf("Circle & Line\n");
        // }

        // if (Collider::isColliding(c1, box, &manifold)) {
        //     printf("Circle & Box1\n");
        // }

        // if (Collider::isColliding(c1, box2, &manifold)) {
        //     printf("Circle & Box2\n");
        // }

        // if (Collider::isColliding(box, box2, &manifold)) {
        //     printf("Box & Box\n");
        //     // color2.rgba.b = 255;
        // }
        // if (Collider::isColliding(box, line, &manifold) || Collider::isColliding(line2, box, &manifold)) {
        //     printf("Box & line\n");
        //     // color2.rgba.b = 255;
        // }

        // // line.end = mpos;

        // if (Collider::isColliding(line, line2, &manifold)) {
        //     printf("line & line2\n");
        //     // color2.rgba.b = 255;
        // }

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

        // mDebug->drawCircle(c1);

        // mDebug->drawCircle(c2);

        // mDebug->drawBox(box);
        // mDebug->drawBox(box2);

        // mDebug->drawLine(lineAB);
        // mDebug->drawLine(lineBC);
        // mDebug->drawLine(lineCD);
        // mDebug->drawLine(lineDA);

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

        if (PUtils::PointInBox(initPos, &box)) {
            isMouseDownInBox = true;
            pos = box.pos;
        }

        if (PUtils::PointInCircle(initPos, (Circle2d*)shapes[0])) {
            isMouseDownInCircle = true;
            pos = shapes[0]->pos;
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