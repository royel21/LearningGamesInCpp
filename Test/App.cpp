#include "App.h"
#include <algorithm>
#include <Graphics/GLheaders.h>
#include <Graphics/DebugRenderer.h>

namespace Plutus
{

    using Axes = std::vector<vec2f>;

    struct Projection {
        float min = 0;
        float max = 0;
    };

    Projection projShapeOn2Axis(vec2f vec, Line L) {
        Projection proj;
        auto vertices = L.getPoints();
        proj.min = vec.dot(vertices[0]);
        proj.max = proj.min;
        for (auto vertex : vertices) {
            auto p = vec.dot(vertex);
            if (p < proj.min) {
                proj.min = p;
            }
            if (p > proj.max) {
                proj.max = p;
            }
        }
        return proj;

    }

    bool sat(Line L1, Line L2) {
        auto axis1 = L1.dir.normal();
        auto axis2 = L2.dir.normal();

        auto proj1 = projShapeOn2Axis(axis1, L1);
        auto proj2 = projShapeOn2Axis(axis1, L2);
        if (std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min) < 0) {
            return false;
        }


        proj1 = projShapeOn2Axis(axis2, L1);
        proj2 = projShapeOn2Axis(axis2, L2);
        if (std::min(proj1.max, proj2.max) - std::max(proj1.min, proj2.min) < 0) {
            return false;
        }

        return true;
    }

    App::App()
    {

    }

    App::App(const char* name, int width, int height)
    {

    }

    App::~App()
    {

    }

    void App::Setup()
    {
        line1 = { 100, 100, 100, 400 };
        line2 = { 90, 420, 400, 420 };

        mDebug = DebugRender::geInstances();
        mDebug->init(&mCamera);
    }

    void App::Update(float dt)
    {

    }

    void App::Draw()
    {
        setBackgoundColor(1, 0, 0, 1);
        auto pos = mInput->getMouseCoords();
        pos.y = 768 - pos.y;

        mDebug->drawLine(line1);
        line2.start = { pos.x, pos.y };
        mDebug->drawLine(line2);

        if (sat(line1, line2)) {
            printf("colliding\n");
        }
        mDebug->render(1);
        mDebug->end();
    }

    void App::Exit()
    {

    }
}