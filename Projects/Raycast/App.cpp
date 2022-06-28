#include "App.h"
#include <Systems/Systems.h>

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Utils/Utils.h>
#include <Graphics/DebugRenderer.h>

#include "ShapeSystem.h"
#include "ShapeComponent.h"

#include <Time/Timer.h>
#include <Math/PMath.h>
#include <Graphics/Graphic.h>
#include <Log/Logger.h>

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

        mDbebug = DebugRender::get();

        mProject.gravity = { 0,0 };

        mSysManager.setProject(&mProject);
        if (true) {
            // auto ent = mProject.scene->createEntity("PhysicTest");

            // auto comp = ent.addComponent<PhysicBodyComponent>();
            // comp->addEdge({ 5,5 }, { mWidth - 10, 1 });
            // comp->addEdge({ mWidth - 10, 5 }, { 1, mHeight - 10 });
            // comp->addEdge({ 5, mHeight - 5 }, { mWidth, 1 });
            // comp->addEdge({ 5,5 }, { 0, mHeight - 10 });

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
            vEnd = PMath::rotateP(vEnd, Start, angle);
        }
        hit = false;
        mPhysicSys->CastRay(Start, vEnd);
        auto p = hit ? mPoint : vEnd;
        points.push_back({ atan2f(p.y - mOrgVec.y, p.x - mOrgVec.x), p });
    }

    void App::Draw()
    {
        if (true) {
            auto pos = mInput->getMouseCoords();

            Vec4f rect = { 0.0f, 0.f, (float)mWidth, (float)mHeight };
            float max = rect.z;

            if (mInput->onKeyDown("MouseLeft")) {
                auto start = Time::micros();

                auto center = pos;
                auto vertices = rect.getvertices();
                mOrgVec = center;

                for (auto& v : vertices) {
                    castRay(center, v);
                }

                auto entities = mPhysicSys->queryWorld(rect);

                if (entities.size()) {

                    for (auto ent : entities)
                    {
                        vertices = mProject.scene->getTransform(ent)->getvertices();
                        for (auto& v : vertices) {
                            castRay(center, v, max);
                            castRay(center, v, max, 0.10f);
                            castRay(center, v, max, -0.10f);
                        }
                    }

                    Logger::info("time: %llu", Time::micros() - start);
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

            // mDbebug->submitCircle(&Circle2d{ pos, 200.0f });
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
        //Render Trangle Fan
        mShader.enable();
        mShader.setUniformMat4("uCamera", mCamera.getCameraMatrix());
        mShader.setUniform4f("uColor", { 1,1,1,1 });
        Graphic::uploadBufferData(mBuffId, mVertices.size() * sizeof(float) * 2, mVertices.data());
        Graphic::bind(mVAO);
        Graphic::drawArrays(GL_TRIANGLE_FAN, mVertices.size());
        Graphic::unBind();
        mShader.disable();
        points.clear();
        mVertices.clear();
        //End Render Trangle Fan
    }

    void App::Exit()
    {

    }
} //End of Plutus namespace