#include "App.h"
#include <algorithm>

#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>

#include "Capsule.h"

#include <Time/Timer.h>

namespace Plutus
{
    App::App(const char* name, int width, int height) : Core(name, width, height)
    {
    }

    App::~App()
    {

    }

    Capsule capsule;

    b2Body* createBody(b2World* world, b2Vec2 pos, const b2Vec2& size, float friction = 1, int type = 0) {
        b2BodyDef myBodyDef;
        myBodyDef.type = (b2BodyType)type;
        myBodyDef.fixedRotation = true;
        myBodyDef.position = pos;

        b2PolygonShape polygonShape;
        b2Vec2 halfSize = { size.x * HMPP, size.y * HMPP };
        polygonShape.SetAsBox(halfSize.x, halfSize.y, halfSize, 0); //a 2x2 rectangle
        //fixture definition
        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &polygonShape;
        myFixtureDef.density = 1.0f;
        // myFixtureDef.restitution = 0;
        if (type == 2) {
            myBodyDef.linearDamping = 2;
            myFixtureDef.friction = friction;
        }

        auto b = world->CreateBody(&myBodyDef);
        b->CreateFixture(&myFixtureDef);
        return b;
    }

    void createCapsule(b2World* world, vec2f position, vec2f dimension, float friction) {

        // // Make the body
        // b2BodyDef bodyDef;
        // bodyDef.type = b2_dynamicBody;
        // bodyDef.position.Set(position.x, position.y);
        // bodyDef.fixedRotation = true;
        // m_body = world->CreateBody(&bodyDef);

        // // Create the box
        // b2PolygonShape boxShape;
        // boxShape.SetAsBox(dimensions.x / 2.0f, (dimensions.y - dimensions.x) / 2.0f);

        // b2FixtureDef fixtureDef;
        // fixtureDef.shape = &boxShape;
        // fixtureDef.density = 1;
        // fixtureDef.friction = friction;
        // m_fixtures[0] = m_body->CreateFixture(&fixtureDef);

        // // Create the circles
        // b2CircleShape circleShape;
        // circleShape.m_radius = dimensions.x / 2.0f;

        // b2FixtureDef circleDef;
        // circleDef.shape = &circleShape;
        // circleDef.density = 1;
        // circleDef.friction = friction;

        // // Bottom circle
        // circleShape.m_p.Set(0.0f, (-dimensions.y + dimensions.x) / 2.0f);
        // m_fixtures[1] = m_body->CreateFixture(&circleDef);

        // // Top Circle
        // circleShape.m_p.Set(0.0f, (dimensions.y - dimensions.x) / 2.0f);
        // m_fixtures[1] = m_body->CreateFixture(&circleDef);
    }

    Box2d* App::createBox(float x, float y, float w, float h, int type, float friction, ShapeType shape) {
        mBoxes.emplace_back(x, y, w, h);
        mBoxes.back().body = createBody(mWorld.get(), toWorld(mBoxes.back().pos), tobVec2(mBoxes.back().size), friction, type);
        mBoxes.back().type = shape;
        return &mBoxes[mBoxes.size() - 1];
    }

    void App::Setup()
    {
        mDebug = DebugRender::geInstances();
        mDebug->init(&mCamera);
        mWorld = std::make_unique<b2World>(b2Vec2{ 0, -10.0f });


        createBox(15, 30, 64, 64, 2, 0.3f);
        createBox(704, 384, 64, 64, 2);
        createBox(5, 5, 1260, 20);
        createBox(1420, 100, 350, 20);
        createBox(910, 195, 350, 20);
        createBox(910 - 520, 195, 350, 20);

        createBox(-300, 50, 260, 20);

        capsule.create(40, 200, 64, 128);

        // auto& box = mBoxes[0];
        // auto pos = box.body->GetPosition();
        // b2CircleShape circleShape;
        // circleShape.m_radius = box.size.x * 0.5f * HMPP;
        // circleShape.m_p = { pos.x * 0.5f, pos.y * 0.5f };

        // // polygonShape.SetAsBox(halfSize.x, halfSize.y, halfSize, 0); //a 2x2 rectangle
        // //fixture definition
        // b2FixtureDef myFixtureDef;
        // myFixtureDef.shape = &circleShape;
        // myFixtureDef.density = 1.0f;
        // myFixtureDef.restitution = 0;

        // box.body->CreateFixture(myFixtureDef);
    }

    uint64_t time = 0;
    float force = 0.2f;
    float cscale = 1;
    float force2 = 20.0f;
    void App::Update(float dt)
    {
        if (Input::get()->onKeyPressed("R")) {
            cscale = 1;
        }
        auto* box = &mBoxes[0];
        auto body = box->body;
        auto currentSpeed = body->GetLinearVelocity();

        if (Input::get()->onKeyDown("J")) {
            body->ApplyForceToCenter(b2Vec2(-force2, 0.0), true);
        }
        else if (Input::get()->onKeyDown("L")) {
            body->ApplyForceToCenter(b2Vec2(force2, 0.0), true);
        }
        // else {
        //     body->SetLinearVelocity(b2Vec2(currentSpeed.x * 0.95f, currentSpeed.y));
        // }

        float MAX_SPEED = 5.0f;
        if (currentSpeed.x < -MAX_SPEED) {
            body->SetLinearVelocity(b2Vec2(-MAX_SPEED, currentSpeed.y));
        }
        else if (currentSpeed.x > MAX_SPEED) {
            body->SetLinearVelocity(b2Vec2(MAX_SPEED, currentSpeed.y));
        }

        if (Input::get()->onKeyPressed("A") || Input::get()->onKeyPressed("S")) {
            body->ApplyLinearImpulseToCenter({ 0, 3.0f }, true);
        }

        if (Input::get()->onKeyDown("+")) {
            cscale += 0.05f;
        }
        if (Input::get()->onKeyDown("-")) {
            cscale -= 0.05f;
        }
        mCamera.setScale(cscale);

        auto cPos = mCamera.getPosition();
        if (Input::get()->onKeyDown("Right")) {
            cPos.x += 5;
        }
        if (Input::get()->onKeyDown("Left")) {
            cPos.x -= 5;
        }
        if (Input::get()->onKeyDown("Up")) {
            cPos.y += 5;
        }
        if (Input::get()->onKeyDown("Down")) {
            cPos.y -= 5;
        }
        mCamera.setPosition(cPos);

        mWorld->Step(timeStep, velIter, posIter);

        for (auto& box : mBoxes) {
            box.update();
        }

        auto size = mCamera.getScaleScreen();
        mCamera.setPosition({ box->pos.x - size.x / 2, box->pos.y - size.y * 0.15f });
    }

    void App::Draw()
    {
        setBackgoundColor(0, 0, 0, 1);

        for (auto& box : mBoxes) {
            mDebug->drawBox(box);
        }

        // mDebug->drawBox(Box2d(capsule.x, capsule.y, capsule.w, capsule.h));
        mDebug->drawBox(capsule.getBox(), { 0xff0000ff });
        mDebug->drawCircle(capsule.getBCircle());
        mDebug->drawCircle(capsule.getTCircle());

        mDebug->drawLine({ 0,0 }, { 1280, 768 });
        mDebug->drawLine({ 0, 768 }, { 1280, 0 });

        mDebug->render();
        mDebug->end();
    }

    void App::Exit()
    {

    }
}