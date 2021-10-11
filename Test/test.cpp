
#include <Graphics/GLheaders.h>
#include <glm/glm.hpp>
#include <Graphics/Shader.h>
#include <Core/Window.h>
#include <Graphics/Camera2D.h>
#include <Graphics/DebugRenderer.h>
#include <Input/Input.h>

#include <Physics/Shapes.h>
#include <Time/Limiter.h>

#include <typeinfo>
#include <string>
#include <Algorithm>

using namespace Plutus;


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

int main(int argc, char** argv) {
    Window window("Custom Circle", 1280, 768);
    Limiter limiter(60.0f);

    Camera2D camera;
    camera.init(1280, 768);

    auto debug = DebugRender::geInstances();
    debug->init(&camera);

    auto input = Input::getInstance();

    Circle circle(0, 0, 80);
    Line line1(100, 100, 100, 400);
    Line line2(90, 420, 400, 420);


    while (window.isFinish()) {
        auto dt = limiter.start();
        printf("dt: %0.4f\n", dt);
        auto pos = input->getMouseCoords();
        pos.y = 768 - pos.y;
        circle.pos = { pos.x, pos.y };

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1, 0, 0, 1);
        // if (camera.isBoxInView({ pos.x, pos.y, 0,0 }, -10)) {
            // debug->drawCircle(circle);
            // debug->drawLine(pos, { pos.x + 80.0f, pos.y - 80.0f });
            // debug->drawCircle({ 1280 / 2, 768 / 2, 100 });

        debug->drawLine(line1);
        line2.start = { pos.x, pos.y };
        debug->drawLine(line2);

        if (sat(line1, line2)) {
            printf("colliding\n");
        }
        debug->render(1);
        debug->end();
        // }

        window.update();
        limiter.end();
    }
}

