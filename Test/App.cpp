#include "App.h"
#include <algorithm>

#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>

#include <Time/Timer.h>

constexpr float PIXEL_SIZE = 100.0f;
//Pixel Per Meter
constexpr float PPM = 1 / PIXEL_SIZE;
//Meter Per Pixel
constexpr float MPP = PIXEL_SIZE;
//Half Meter Per Pixel
constexpr float HMPP = PPM / 2.0f;


namespace Plutus
{
    b2Vec2 toWorld(const vec2f& value) {
        return { value.x * PPM, value.y * PPM };
    }

    vec2f fromWorld(b2Vec2 value) {
        return { value.x * MPP, value.y * MPP };
    }

    b2Vec2 tobVec2(const vec2f& vec) { return { vec.x, vec.y }; }

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

    b2Body* createBody(b2World* world, b2Vec2 pos, const b2Vec2& size, int type) {
        b2BodyDef myBodyDef;
        myBodyDef.type = (b2BodyType)type;
        myBodyDef.fixedRotation = true;
        myBodyDef.position = pos;
        myBodyDef.gravityScale = 0;

        myBodyDef.linearDamping = 10;
        myBodyDef.angularDamping = 10;

        b2PolygonShape polygonShape;
        polygonShape.SetAsBox(size.x, size.y, size, 0); //a 2x2 rectangle
        //fixture definition
        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &polygonShape;
        myFixtureDef.density = 10.0f;
        myFixtureDef.friction = 10;
        myFixtureDef.restitution = 0;

        auto b = world->CreateBody(&myBodyDef);
        b->CreateFixture(&myFixtureDef);

        return b;
    }

    void App::Setup()
    {
        mDebug = DebugRender::geInstances();
        mDebug->init(&mCamera);
        mWorld = std::make_unique<b2World>(b2Vec2{ 0, 0 });



        bFloor.pos = { 10, 10 };
        bFloor.size = { 1260, 64 };
        floor = createBody(mWorld.get(), toWorld(bFloor.pos), { bFloor.size.x * HMPP, bFloor.size.y * HMPP }, 0);


        box.pos = { 640, 384 };
        box.size = { 64, 64 };

        body = createBody(mWorld.get(), toWorld(box.pos), { box.size.y * HMPP, box.size.y * HMPP }, 2);


        box2.pos = { 704, 384 };
        box2.size = { 64,64 };
        body2 = createBody(mWorld.get(), toWorld(box2.pos), { box2.size.y * HMPP, box2.size.y * HMPP }, 2);
    }

    uint64_t time = 0;
    float force = PIXEL_SIZE * 0.05f;

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

        bFloor.pos = fromWorld(floor->GetPosition());
        box.pos = fromWorld(body->GetPosition());
        box2.pos = fromWorld(body2->GetPosition());
    }

    void App::Draw()
    {
        setBackgoundColor(0, 0, 0, 1);

        mDebug->drawBox(bFloor);
        mDebug->drawBox(box);
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