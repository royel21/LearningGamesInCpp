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
#include <Graphics/Graphic.h>

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
        mShader.init("Projects/TestBed/ver.glsl", "Projects/TestBed/frag.glsl");

        mVAO = Graphic::createVertexArray();
        mBuffId = Graphic::createBufferArray();

        Graphic::setFAttribute(0, 2, 2 * sizeof(float));

        Graphic::unBind();

        mEntities.reserve(50);

        mDbebug = DebugRender::get();

        mProject.gravity = { 0,0 };

        mSysManager.setProject(&mProject);
        if (false) {
            for (size_t i = 0; i < 25; i++)
            {
                auto ent = mProject.scene->createEntity("PhysicTest - " + std::to_string(i));
                float x = (float)Utils::getRandom(10, 1100);
                float y = (float)Utils::getRandom(10, 620);

                ent.addComponent<TransformComponent>(x, y, 60, 60);

                auto comp = ent.addComponent<RigidBodyComponent>();
                comp->addBox({ 0,0 }, { 60, 60 });
            }
        }
        else {
            auto ent = mProject.scene->createEntity("PhysicTest");
            ent.addComponent<TransformComponent>(300.0f, 300.0f, 60, 60);

            auto comp = ent.addComponent<RigidBodyComponent>();
            comp->addBox({ 0,0 }, { 60, 60 });

            ent = mProject.scene->createEntity("PhysicTest");
            ent.addComponent<TransformComponent>(270.0f, 200.0f, 60, 60);

            comp = ent.addComponent<RigidBodyComponent>();
            comp->addBox({ 0,0 }, { 60, 60 });

            ent = mProject.scene->createEntity("PhysicTest");
            ent.addComponent<TransformComponent>(360.0f, 200.0f, 60, 60);

            comp = ent.addComponent<RigidBodyComponent>();
            comp->addBox({ 0,0 }, { 60, 60 });
        }

        mPhysicSys = mSysManager.AddSystem<PhysicSystem>();

        mSysManager.AddSystem<DebugSystem>(&mCamera);
        mSysManager.AddSystem<ShapeSystem>(&mCamera);

        mSysManager.init();

        mPhysicSys->mRayCallBack = [&](b2Fixture* fixture, Vec2f point, Vec2f normal, float fraction) -> float {
            if (mDist > fraction) {
                mDist = fraction;
                mPoint = point;
                hit = true;
            }
            return 1;
        };

        mPhysicSys->mQueryCallBack = [&](b2Fixture* fixture) -> bool {

            auto entId = fixture->GetBody()->GetUserData().pointer;
            mEntities.push_back(entId);
            return 1.0f;
        };

        // ent = mProject.scene->createEntity("shapec");
        // auto shape = ent.addComponent<ShapeComponent>();
        // shape->pos = { 10, 10 };
        // shape->Points.push_back({ 100, 30 });
        // shape->Points.push_back({ 50, 50 });
    }


    void App::Update(float dt)
    {
        mSysManager.update(dt);
    }

    Vec2f start;
    Vec2f end;

    void App::castRay(const Vec2f& Start, const Vec2f& end, float max = 0, float angle) {
        mDist = 1;
        auto vEnd = end;

        if (max) {
            auto dir = (end - Start).unit();
            vEnd = Start + (dir * max);
        }

        if (angle != 0) {
            vEnd = rotateP(vEnd, Start, angle);
        }
        hit = false;
        mPhysicSys->CastRay(Start, vEnd);
        auto p = hit ? mPoint : vEnd;
        points.push_back({ atan2f(p.y, p.x), p });
    }

    void App::Draw()
    {
        if (true) {
            auto pos = mInput->getMouseCoords();

            Vec4f rect = { pos.x - 200, pos.y - 200, 400, 400 };
            float max = rect.z * 0.5f;

            if (mInput->onKeyDown("MouseLeft")) {
                auto start = Time::micros();

                auto center = rect.getCenter();
                auto vertices = rect.getvertices();

                for (auto& v : vertices) {
                    castRay(center, v);
                }

                mPhysicSys->queryWorld(rect);

                if (mEntities.size()) {

                    for (auto ent : mEntities)
                    {
                        vertices = mProject.scene->getTransform(ent)->getvertices();
                        for (auto& v : vertices) {
                            castRay(center, v, max);
                            castRay(center, v, max, 0.10f);
                            castRay(center, v, max, -0.10f);
                        }
                    }

                    Logger::info("time: %llu", Time::micros() - start);

                    mEntities.clear();
                }

                std::sort(points.begin(), points.end(), [](const auto& p1, const auto p2)-> bool {
                    return std::get<0>(p1) < std::get<0>(p2);
                    });

                mVertices.push_back(center);
                for (auto [f, p] : points) {
                    mDbebug->submitLine(center, p);
                    mVertices.push_back(p);
                }
                mVertices.push_back(mVertices[1]);
            }

            mDbebug->submitCircle(&Circle2d{ pos, 200.0f });
        }
        else {
            if (mInput->onKeyDown("MouseLeft")) {
                end = mInput->getMouseCoords();;
                if (start != end) {
                    castRay(start, end);
                    Logger::info("next");
                }
            }

            if (mInput->onKeyPressed("MouseRight")) {
                start = mInput->getMouseCoords();
            }
            mDbebug->submitLine(start, end);
            mDbebug->submitLine(start, mPoint);
        }

        mDbebug->end();
        mDbebug->render();

        mShader.enable();
        mShader.setUniformMat4("uCamera", mCamera.getCameraMatrix());
        mShader.setUniform4f("uColor", { 1,1,1,1 });


        // mBuffer.push_back({ 200,200 });
        // mBuffer.push_back({ 400,200 });
        // mBuffer.push_back({ 200,400 });
        // mBuffer.push_back({ 0,200 });
        // mBuffer.push_back({ 200,0 });
        // mBuffer.push_back({ 400,200 });

        Graphic::uploadBufferData(mBuffId, mVertices.size() * sizeof(float) * 2, mVertices.data());

        Graphic::bind(mVAO);

        Graphic::drawArrays(GL_TRIANGLE_FAN, mVertices.size());

        Graphic::unBind();
        mShader.disable();
        mBuffer.clear();
        points.clear();
        mVertices.clear();
    }

    void App::Exit()
    {

    }
} //End of Plutus namespace