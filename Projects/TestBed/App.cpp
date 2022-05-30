#include "App.h"
#include <Systems/Systems.h>

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Utils/Utils.h>
#include <Graphics/DebugRenderer.h>

#include <Math/LogVec.h>

#include "ShapeSystem.h"
#include "ShapeComponent.h"

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
        mDbebug = DebugRender::get();

        mProject.gravity = { 0,0 };

        mSysManager.setProject(&mProject);
        for (size_t i = 0; i < 15; i++)
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
            Math::Log(point);
            auto entId = fixture->GetBody()->GetUserData().pointer;
            Logger::info("Ent: %i frac:%0.4f", entId, fraction);
            return 1.0f;
        };

        auto ent = mProject.scene->createEntity("shapec");
        auto shape = ent.addComponent<ShapeComponent>();
        shape->pos = { 10, 10 };
        shape->Points.push_back({ 100, 30 });
        shape->Points.push_back({ 50, 50 });
        // shape->Points.push_back({ 0, 100 });
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

        mDbebug->drawOneLine(start, end);
    }

    void App::Exit()
    {

    }
} //End of Plutus namespace