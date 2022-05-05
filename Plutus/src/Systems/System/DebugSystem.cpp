#include "DebugSystem.h"

#include <box2d/box2d.h>

#include <ECS/Scene.h>
#include <ECS/Components.h>
#include <Graphics/Camera2D.h>
#include <Graphics/DebugRenderer.h>

#include <Core/Project.h>

namespace Plutus
{
    void DebugSystem::init(Project* project)
    {
        mProject = project;
        mDebug = DebugRender::get();
        mDebug->init(mCamera);
    }

    void DebugSystem::update(float dt)
    {
        auto rigidview = mProject->scene->getRegistry()->view<TransformComponent, RigidBodyComponent>();
        for (auto [e, trans, rbody] : rigidview.each()) {

            for (auto& fixture : rbody.mFixtures) {
                auto pos = fromWorld(rbody.mBody->GetPosition());

                switch (fixture.type) {
                case BoxShape: {
                    Vec4f rect = { pos + fixture.offset, fixture.size.x, fixture.size.y };
                    if (mCamera->isBoxInView(rect))
                    {
                        mDebug->drawBox(rect);
                    }
                    break;
                }
                case EdgeShape: {
                    mDebug->drawLine(pos + fixture.offset, fixture.size + fixture.offset);
                    break;
                }
                case CircleShape: {
                    Vec4f rect = { pos.x, pos.y, fixture.radius, fixture.radius };
                    if (mCamera->isBoxInView(rect))
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