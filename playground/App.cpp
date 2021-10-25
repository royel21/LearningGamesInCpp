#include "App.h"
#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>

#include <Math/Vectors.h>
#include <Physics/Collider.h>
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
    vec2f initPos;
    vec2f cpos;
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

    vec2f getProjection(const std::vector<Plutus::vec2f>& verts, vec2f axis) {
        vec2f result;

        result.x = axis.dot(verts[0]);
        result.y = result.x;
        for (const vec2f& v : verts) {
            float proj = axis.dot(v);

            if (proj < result.x) result.x = proj;
            if (proj > result.y) result.y = proj;
        }
        return result;
    }

    bool testVertices(const std::vector<Plutus::vec2f>& verts1, const std::vector<Plutus::vec2f>& verts2) {
        auto b1face1 = (verts1[1] - verts1[0]).normal();
        auto b1face2 = (verts1[2] - verts1[1]).normal();
        auto b2face1 = (verts2[1] - verts2[0]).normal();
        auto b2face2 = (verts2[2] - verts2[1]).normal();

        std::vector<vec2f> axices = {
            b1face1,
            b1face2,
            b2face1,
            b2face2
        };
        for (auto& axis : axices) {
            auto  proj1 = getProjection(verts1, axis);
            auto  proj2 = getProjection(verts2, axis);
            if (proj1.x >= proj2.y || proj2.x >= proj1.y) {
                return true;
            }
        }
        return false;
    }
    // Box Vs Box
    bool isColliding(Box2d& b1, Box2d& b2)
    {
        auto verts1 = b1.getVertices();
        auto verts2 = b2.getVertices();

        if (b1.rotation || b2.rotation && 0) {
            vec2f result;

            if (testVertices(verts1, verts2)) return false;
            if (testVertices(verts2, verts1)) return false;

            return true;
        }
        else {
            for (auto p : verts2) {
                if (Collider::pointBox(p, b1)) return true;
            }

            for (auto p : verts1) {
                if (Collider::pointBox(p, b2)) return true;
            }
            return false;
        }
    }
    void App::Draw()
    {
        if (isMouseDown) {
            auto dis = initPos - mpos;
            box.pos = cpos - dis;
        }

        if (Collider::isColliding(c1, c2)) {
            // color.rgba.r = 255;
        }

        if (Collider::isColliding(line, c1)) {
            // color2.rgba.g = 255;
        }

        if (Collider::isColliding(c1, box)) {
            printf("circle in box collision\n");
        }

        if (isColliding(box, box2)) {
            printf("circle in box2 collision\n");
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
        cpos = box.pos;
        if (Collider::pointBox(initPos, box))
            isMouseDown = true;
    }

    void App::onKeyUp(const std::string& key)
    {
        isMouseDown = false;
    }

    void App::onMouseMove(float x, float y)
    {
        mpos = Input::get()->getMouseCoords();
    }
}