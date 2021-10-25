#include "App.h"
#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>

#include <Math/Vectors.h>
#include <Physics/Collider.h>
#include <Physics/PUtils.h>
#include <Physics/Shapes.h>

#include <limits>
#include <algorithm>

namespace Plutus
{
    App::App(const char* name, int width, int height) : Core(name, width, height)
    {
        Input::get()->addEventListener(this);
    }

    void App::Setup()
    {
        mDebug = DebugRender::get();
        mDebug->init(&mCamera);

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
    Line2d line2 = { 200,200, 200, 600 };
    float speed = 5;
    void App::Update(float dt)
    {
        if (Input::get()->onKeyDown("Up")) { c1.pos.y += speed; }
        if (Input::get()->onKeyDown("Down")) { c1.pos.y -= speed; }
        if (Input::get()->onKeyDown("Right")) { c1.pos.x += speed; }
        if (Input::get()->onKeyDown("Left")) { c1.pos.x -= speed; }
    }


    Box2d box(50, 50, 100, 100, 45);
    // Box2d box3(50, 50, 100, 100);
    Box2d box2(50, 200, 100, 100);

    void App::Draw()
    {
        if (isMouseDownInBox) {
            auto dis = initPos - mpos;
            box.pos = pos - dis;
        }
        if (isMouseDownInCircle) {
            auto dis = initPos - mpos;
            c1.pos = pos - dis;
        }

        if (Collider::isColliding(c1, c2, true)) {
            printf("Circle & Circle\n");
        }

        if (Collider::isColliding(line, c1, true) || Collider::isColliding(c1, line2, true)) {
            printf("Circle & Line\n");
        }

        if (Collider::isColliding(c1, box, true)) {
            printf("Circle & Box1\n");
        }

        if (Collider::isColliding(c1, box2, true)) {
            printf("Circle & Box2\n");
        }

        if (Collider::isColliding(box, box2, true)) {
            printf("Box & Box\n");
            // color2.rgba.b = 255;
        }
        if (Collider::isColliding(box, line, true) || Collider::isColliding(line2, box, true)) {
            printf("Box & line\n");
            // color2.rgba.b = 255;
        }

        // line.end = mpos;

        if (Collider::isColliding(line, line2, true)) {
            printf("line & line2\n");
            // color2.rgba.b = 255;
        }

        mDebug->drawCircle(c1);

        mDebug->drawCircle(c2);

        mDebug->drawBox(box);
        mDebug->drawBox(box2);
        // mDebug->drawBox(box3);

        mDebug->drawLine(line);
        mDebug->drawLine(line2);

        mDebug->render();
        mDebug->end();
    }

    void App::Exit()
    {

    }

    void App::onKeyDown(const std::string& key)
    {
        initPos = Input::get()->getMouseCoords();

        if (PUtils::PointInBox(initPos, box)) {
            isMouseDownInBox = true;
            pos = box.pos;
        }

        if (PUtils::PointInCircle(initPos, c1)) {
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