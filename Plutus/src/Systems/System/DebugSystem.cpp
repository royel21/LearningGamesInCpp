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
        auto view = mScene->getRegistry()->view<Transform, RigidBody>();
        for (auto [e, trans, rbody] : view.each()) {

            for (auto& fixture : rbody.mFixtures) {
                auto pos = fromWorld(rbody.mBody->GetPosition());
                vec4f rect = { pos.x, pos.y, fixture.size.x, fixture.size.y };
                if (mCamera->isBoxInView(rect, 200))
                {
                    switch (fixture.type) {
                    case PBox: {
                        mDebug->drawBox(rect);
                        break;
                    }
                    case PLine: {
                        mDebug->drawLine(pos, fixture.size);
                        break;
                    }
                    case PCircle: {
                        mDebug->drawCircle(pos, fixture.radius);
                        break;
                    }
                    }
                }
            }
        }
        mDebug->render();
        mDebug->end();
    }
}