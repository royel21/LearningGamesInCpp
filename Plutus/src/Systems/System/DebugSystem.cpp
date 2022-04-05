#include "DebugSystem.h"


#include <ECS/Scene.h>
#include <ECS/Components.h>
#include <Graphics/Camera2D.h>
#include <Graphics/DebugRenderer.h>

// #include <Graphi

namespace Plutus
{
    void DebugSystem::init()
    {
        mDebug = DebugRender::get();
        mDebug->init(mCamera);
    }

    void DebugSystem::update(float dt)
    {
        auto view = mScene->getRegistry()->view<TransformComponent, RigidBodyComponent>();
        for (auto [e, trans, rbody] : view.each()) {

            for (auto& fixture : rbody.mFixtures) {
                auto pos = fromWorld(rbody.mBody->GetPosition());

                switch (fixture.type) {
                case BoxShape: {
                    vec4f rect = { pos + fixture.offset, fixture.size.x, fixture.size.y };
                    if (mCamera->isBoxInView(rect, 200))
                    {
                        mDebug->drawBox(rect);
                    }
                    break;
                }
                case EdgeShape: {
                    mDebug->drawLine(pos, fixture.size);
                    break;
                }
                case CircleShape: {
                    vec4f rect = { pos.x, pos.y, fixture.radius, fixture.radius };
                    if (mCamera->isBoxInView(rect, 200))
                    {
                        mDebug->drawCircle(pos + fixture.offset, fixture.radius);
                    }
                    break;
                }
                }

            }
        }
        mDebug->end();
        mDebug->render();

        if (mIsDrawGrid) {
            mDebug->drawGrid();
        }
    }

    void DebugSystem::drawGrid(bool isDraw, int cellW, int cellH) {
        mDebug = DebugRender::get();
        mIsDrawGrid = isDraw;
        mDebug->setCellSize({ cellW, cellH });
    }
}