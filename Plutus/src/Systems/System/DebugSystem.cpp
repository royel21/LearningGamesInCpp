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

    void DebugSystem::prepare(PhysicBodyComponent pc) {
        for (auto& fixture : pc.mFixtures) {
            auto pos = fromWorld(pc.mBody->GetPosition());

            switch (fixture.type) {
            case BoxShape: {
                Vec4f rect = { pos + fixture.offset, fixture.size.x, fixture.size.y };
                if (mCamera->getViewPortDim().overlap(rect))
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
                if (mCamera->getViewPortDim().overlap(rect))
                {
                    mDebug->drawCircle(pos + fixture.offset, fixture.radius);
                }
                break;
            }
            }

        }
    }

    void DebugSystem::update(float dt)
    {
        auto rigidview = mProject->scene->getRegistry()->view<RigidBodyComponent>();
        for (auto [e, rbody] : rigidview.each()) {
            prepare(rbody);
        }

        auto physicview = mProject->scene->getRegistry()->view<PhysicBodyComponent>();
        for (auto [e, rbody] : physicview.each()) {
            prepare(rbody);
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