#include "App.h"
#include <algorithm>

#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>

#include <Time/Timer.h>

constexpr float PIXEL_SIZE = 64.0f;
//Pixel Per Meter
constexpr float PPM = 1 / PIXEL_SIZE;
//Meter Per Pixel
constexpr float MPP = PIXEL_SIZE;
//Half Meter Per Pixel
constexpr float HMPP = PPM / 2.0f;

namespace Plutus
{
    App::App(const char* name, int width, int height) : Core(name, width, height)
    {
    }

    App::~App()
    {

    }
    Box2d box;
    Box2d box2;
    Box2d bFloor;

    b2Body* body;
    b2Body* body2;
    b2Body* floor;
    b2Vec2 position{ 10.0f - 1.0f, 12.0f - 0.5f };

    void App::Setup()
    {
        mDebug = DebugRender::geInstances();
        mDebug->init(&mCamera);
        mWorld = std::make_unique<b2World>(b2Vec2{ 0, 0 });


        box.pos = { 1280 / 2 - 32, 768 / 2 - 32 };
        box.size = { 64,64 };

        box2.pos = { (1280 / 2 - 32) + 64, (768 / 2 - 32) + 64 };
        box2.size = { 64,64 };

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;
        myBodyDef.fixedRotation = true;
        myBodyDef.position.Set(box.pos.x * PPM, box.pos.y * PPM);
        myBodyDef.gravityScale = 0;

        //shape definition
        b2PolygonShape polygonShape;

        //fixture definition
        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &polygonShape;
        myFixtureDef.density = 10.0f;
        myFixtureDef.friction = 10;
        myFixtureDef.restitution = 0;

        body = mWorld->CreateBody(&myBodyDef);
        body->SetFixedRotation(true);

        polygonShape.SetAsBox(box.size.x * HMPP, box.size.y * HMPP); //a 2x2 rectangle
        body->CreateFixture(&myFixtureDef);


        myBodyDef.position.Set(box2.pos.x * PPM, box2.pos.y * PPM);
        polygonShape.SetAsBox(box.size.x * HMPP, box.size.y * HMPP); //a 2x2 rectangle
        //create dynamic body
        body2 = mWorld->CreateBody(&myBodyDef);
        body2->CreateFixture(&myFixtureDef);
        //a static body

        bFloor.pos = { 10, 10 };
        bFloor.size = { 768, 64 };

        myBodyDef.type = b2_staticBody;
        myBodyDef.position.Set(bFloor.pos.x * PPM, bFloor.pos.y * PPM);
        floor = mWorld->CreateBody(&myBodyDef);
        //ground
        polygonShape.SetAsBox(bFloor.size.x * HMPP, bFloor.size.y * HMPP);
        floor->CreateFixture(&myFixtureDef);
    }

    uint64_t time = 0;
    float force = 6.0f;

    void App::Update(float dt)
    {
        if (Input::get()->onKeyPressed("R")) {
            body->SetTransform(position, body->GetAngle());
            time = Timer::millis();
        }

        if (Input::get()->onKeyDown("A")) {
            body->ApplyLinearImpulseToCenter({ -force, 0 }, true);
        }
        if (Input::get()->onKeyDown("D")) {
            body->ApplyLinearImpulseToCenter({ force, 0 }, true);
        }
        if (Input::get()->onKeyDown("W")) {
            body->ApplyLinearImpulseToCenter({ 0, force }, true);
        }
        if (Input::get()->onKeyDown("S")) {
            body->ApplyLinearImpulseToCenter({ 0, -force }, true);
        }

        mWorld->Step(timeStep, velIter, posIter);
    }

    void App::Draw()
    {
        setBackgoundColor(1, 0, 0, 1);
        auto floorPos = floor->GetPosition();
        bFloor.pos = { floorPos.x * MPP, floorPos.y * MPP };
        mDebug->drawBox(bFloor);

        // auto pos = body->GetPosition();
        // mDebug->drawBox(glm::vec4{ pos.x * MPP - 64.0f, pos.y * MPP - 64.0f, 64.0f, 64.0f });
        auto pos2 = body->GetPosition();
        box.pos = { pos2.x * MPP, pos2.y * MPP };
        mDebug->drawBox(box);

        pos2 = body2->GetPosition();
        box2.pos = { pos2.x * MPP, pos2.y * MPP };
        mDebug->drawBox(box2);

        mDebug->drawLine({ 0,0 }, { 1280, 768 });
        mDebug->drawLine({ 0, 768 }, { 1280, 0 });


        mDebug->render();
        mDebug->end();
    }

    void App::Exit()
    {

    }
}