#include "App.h"
#include <Systems/Systems.h>

#include <ECS/Scene.h>
#include <ECS/Components.h>

#include <Utils/Utils.h>
#include <Graphics/DebugRenderer.h>

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

        mDbebug = DebugRender::get();

        mProject.gravity = { 0,0 };

        mSysManager.setProject(&mProject);
        /****************************/
        auto ent = mProject.scene->createEntity("PhysicTest");
        ent.addComponent<TransformComponent>(300.0f, 300.0f, 60, 60);

        auto comp = ent.addComponent<RigidBodyComponent>();
        auto fix = comp->addBox({ 0, 0 }, { 100, 30 });

        /****************************/

        mPhysicSys = mSysManager.AddSystem<PhysicSystem>();

        mSysManager.AddSystem<DebugSystem>(&mCamera);

        mSysManager.init();

        // fix->b2fixture.setR

    }

    void App::Update(float dt)
    {
        mSysManager.update(dt);
    }

    Vec2f start;
    Vec2f end;

    void App::Draw()
    {

    }

    void App::Exit()
    {

    }
} //End of Plutus namespace