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

    void App::Update(float dt)
    {

    }

    Circle2d c1 = { 640,384, 50 };
    Circle2d c2 = { 320,384, 25 };
    Line2d line = { 200,300, 200, 750 };

    Box2d box(50, 50, 100, 100, 45);
    // Box2d box3(50, 50, 100, 100);
    Box2d box2(50, 50, 100, 100);

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

        if (Collider::isColliding(c1, c2)) {
            printf("Circle & Circle\n");
        }

        if (Collider::isColliding(line, c1)) {
            printf("Circle & Line\n");
        }

        if (Collider::isColliding(c1, box)) {
            printf("Circle & Box1\n");
        }

        if (Collider::isColliding(c1, box2)) {
            printf("Circle & Box2\n");
        }

        if (Collider::isColliding(box, box2)) {
            printf("Box & Box\n");
            // color2.rgba.b = 255;
        }

        mDebug->drawCircle(c1);

        mDebug->drawCircle(c2);

        mDebug->drawBox(box);
        mDebug->drawBox(box2);
        // mDebug->drawBox(box3);

        mDebug->drawLine(line);

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