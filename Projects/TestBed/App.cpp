#include "App.h"
#include <Systems/Systems.h>

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Utils/Utils.h>
#include <Graphics/DebugRenderer.h>

#include <Math/LogVec.h>

#include "ShapeSystem.h"
#include "ShapeComponent.h"

#include <Time/Timer.h>
#include <Math/PMath.h>

namespace Plutus
{
    App::App() {
        mName = "Box2d Test - Bed";
    }

    App::~App()
    {
    }

    void App::Init()
    {
        mEntities.reserve(50);

        mDbebug = DebugRender::get();

        mProject.gravity = { 0,0 };

        mSysManager.setProject(&mProject);
        for (size_t i = 0; i < 25; i++)
        {
            auto ent = mProject.scene->createEntity("PhysicTest - " + std::to_string(i));
            float x = (float)Utils::getRandom(10, 1100);
            float y = (float)Utils::getRandom(10, 620);

            ent.addComponent<TransformComponent>(x, y, 60, 60);

            auto comp = ent.addComponent<RigidBodyComponent>();
            comp->addBox({ 0,0 }, { 60, 60 });
        }

        mPhysicSys = mSysManager.AddSystem<PhysicSystem>();

        mSysManager.AddSystem<DebugSystem>(&mCamera);
        mSysManager.AddSystem<ShapeSystem>(&mCamera);

        mSysManager.init();

        mPhysicSys->mRayCallBack = [&](b2Fixture* fixture, Vec2f point, Vec2f normal, float fraction) -> float {
            if (fraction == 1) {
                mPoint = point;
                blocked = fraction != 1;
            }
            else {
                blocked = true;
                return 0;
            }

            return 1;
        };

        mPhysicSys->mQueryCallBack = [&](b2Fixture* fixture) -> bool {

            auto entId = fixture->GetBody()->GetUserData().pointer;
            mEntities.push_back(entId);
            return 1.0f;
        };

        auto ent = mProject.scene->createEntity("shapec");
        auto shape = ent.addComponent<ShapeComponent>();
        shape->pos = { 10, 10 };
        shape->Points.push_back({ 100, 30 });
        shape->Points.push_back({ 50, 50 });
    }


    void App::Update(float dt)
    {
        mSysManager.update(dt);
    }

    Vec2f start;
    Vec2f end;

    void App::Draw()
    {
        if (mInput->onKeyDown("MouseLeft")) {
            end = mInput->getMouseCoords();;
            if (start != end) {
                mPhysicSys->CastRay(start, end);
            }
        }

        if (mInput->onKeyPressed("MouseRight")) {
            start = mInput->getMouseCoords();
        }

        auto pos = mInput->getMouseCoords();

        Vec4f rect = { pos.x - 200, pos.y - 200, 400, 400 };

        // if (mInput->onKeyDown("MouseLeft")) {
        //     auto start = Time::micros();
        //     mPhysicSys->queryWorld(rect);
        //     // Logger::info("end Querying: %zu", mEntities.size());

        //     if (mEntities.size()) {

        //         for (auto ent : mEntities)
        //         {
        //             auto vertices = mProject.scene->getTransform(ent)->getvertices();
        //             for (auto& v : vertices) {
        //                 if (rect.contain(v)) {
        //                     blocked = true;
        //                     mPhysicSys->CastRay(rect.getCenter(), v);

        //                     if (!blocked) {
        //                         mVertices.push_back(mPoint);
        //                     }
        //                 }
        //             }
        //         }

        //         Logger::info("time: %llu", Time::micros() - start);

        //         for (auto& v : mVertices) {
        //             mDbebug->submitLine(rect.getCenter(), v);
        //         }

        //         mVertices.clear();
        //         mEntities.clear();
        //     }
        // }
        // Logger::info("points: %zu", mVertices.size());
        // mDbebug->submitBox(rect);
        // mDbebug->end();
        // mDbebug->render();

        // mDbebug->drawLine(start, end);

        // Vec2f p = end;
        // rotate(p, rect.getCenter(), -45);
        // mDbebug->drawLine(start, p);
        start = { mWidth * 0.5f, mHeight * 0.5f };
        end = { mWidth * 1.0f, mHeight * 0.5f };
        mDbebug->submitLine(start, end);
        mDbebug->submitLine(start, rotateP(end, start, .1f));
        mDbebug->submitLine(start, rotateP(end, start, -.1f));


        mDbebug->end();
        mDbebug->render();

        mVertices.clear();
    }

    void App::Exit()
    {

    }
} //End of Plutus namespace