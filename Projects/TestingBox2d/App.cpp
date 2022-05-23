#include "App.h"
#include <algorithm>

#include <Input/Input.h>
#include <Graphics/DebugRenderer.h>
#include <Math/Vectors.h>

#include "Capsule.h"

#include <Time/Timer.h>
#include <Graphics/GLSL.h>
#include <Assets/Assets.h>
#include <ECS/Components/PhysicBodyComponent.h>

namespace Plutus
{
    App::App(const char* name, int width, int height)
    {
        mName = name;
        mWidth = width;
        mHeight = height;
    }

    App::~App()
    {
        for (auto shape : mShapes)
            delete shape;
    }


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

    Capsule capsule;

    void createCapsule(b2World* world) {

        capsule.create(40, 200, 64, 128);

        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;
        myBodyDef.fixedRotation = true;
        myBodyDef.position = toWorld(capsule.position);

        capsule.body = world->CreateBody(&myBodyDef);

        //Radius of the circle
        float r = capsule.size.x * HMPP;

        b2PolygonShape polygonShape;
        b2Vec2 halfSize = { r, capsule.size.y * HMPP * 0.5f };

        polygonShape.SetAsBox(halfSize.x, halfSize.y, halfSize, 0); //a 2x2 rectangle

        b2FixtureDef myFixtureDef;
        myFixtureDef.shape = &polygonShape;
        myFixtureDef.density = 1.0f;
        myFixtureDef.friction = 0.3f;
        myFixtureDef.restitution = 0;

        capsule.body->CreateFixture(&myFixtureDef);

        b2FixtureDef circleDef;
        circleDef.density = 1.0f;
        circleDef.friction = 0.2f;
        //Buttom Circle Shape Def
        b2CircleShape circleShapeB;
        circleShapeB.m_p.Set(r, 0);
        circleShapeB.m_radius = r + 0.008f;

        //Buttom Circle
        circleDef.shape = &circleShapeB;
        capsule.body->CreateFixture(&circleDef);

        //Buttom Circle Shape Def
        b2CircleShape circleShapeA;
        circleShapeA.m_p.Set(r, capsule.size.y * HMPP);
        circleShapeA.m_radius = r;

        circleDef.shape = &circleShapeA;
        capsule.body->CreateFixture(&circleDef);
    }

    void App::createBox(float x, float y, float w, float h, int type, float friction) {
        auto box = new Box2d(x, y, w, h);
        box->body = createBody(mWorld.get(), toWorld(box->pos), tobVec2(box->size), friction, type);
        mShapes.push_back(box);
    }

    void App::createLine(float x1, float y1, float x2, float y2)
    {
        auto line = new Line2d(x1, y1, x2, y2);

        b2BodyDef bd;
        bd.position.Set(0.0f, 0.0f);
        line->body = mWorld->CreateBody(&bd);

        b2EdgeShape shape;
        shape.SetTwoSided(toWorld(line->pos), toWorld(line->end));

        line->body->CreateFixture(&shape, 0.0f);

        mShapes.push_back(line);
    }

    void App::Init()
    {
        mShader.init(GLSL::vertexShader, GLSL::fragShader);
        mBatch.init();
        mBatch.setCamera(&mCamera);
        texture = AssetManager::get()->addAsset<Texture>("player", "assets/textures/Player.png", 60, 64, 64);

        mDebug = DebugRender::get();
        mDebug->init(&mCamera);
        mWorld = std::make_unique<b2World>(b2Vec2{ 0, -10.0f });

        createBox(15, 30, 64, 64, 2, 0.3f);
        createBox(704, 384, 64, 64, DynamicBody);
        createBox(910, 240, 350, 20, StaticBody);

        createBox(890, 230, 350, 20, StaticBody);

        createCapsule(mWorld.get());

        createLine(0, 40, 280, 28);

        createLine(280, 28, 600, 150);

        createLine(600, 150, 750, 150);

        createLine(750, 150, 1050, 28);

        createLine(1050, 28, 1350, 100);


        createLine(0, 200, 1350, 400);
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
        auto body = capsule.body;
        auto currentSpeed = body->GetLinearVelocity();

        if (Input::get()->onKeyDown("Left")) {
            body->ApplyForceToCenter(b2Vec2(-force2, 0.0), true);
        }
        else if (Input::get()->onKeyDown("Right")) {
            body->ApplyForceToCenter(b2Vec2(force2, 0.0), true);
        }
        else {
            body->SetLinearVelocity(b2Vec2(currentSpeed.x * 0.95f, currentSpeed.y));
        }

        float MAX_SPEED = 5.0f;
        if (currentSpeed.x < -MAX_SPEED) {
            body->SetLinearVelocity(b2Vec2(-MAX_SPEED, currentSpeed.y));
        }
        else if (currentSpeed.x > MAX_SPEED) {
            body->SetLinearVelocity(b2Vec2(MAX_SPEED, currentSpeed.y));
        }

        if (Input::get()->onKeyPressed("A") || Input::get()->onKeyPressed("S")) {
            body->ApplyLinearImpulseToCenter({ 0, 4.0f }, true);
        }

        mWorld->Step(timeStep, velIter, posIter);

        for (auto shape : mShapes) {
            shape->update();
        }

        capsule.update();

        if (Input::get()->isCtrl) {
            if (Input::get()->onKeyDown("+")) {
                cscale += 0.05f;
                mCamera.setScale(cscale);
            }
            if (Input::get()->onKeyDown("-")) {
                cscale -= 0.05f;
                mCamera.setScale(cscale);
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
        }
        else {
            auto size = mCamera.getScaleScreen();
            mCamera.setPosition(Vec2f{ capsule.position.x - size.x * 0.5f, capsule.position.y - size.y * 0.15f });
        }

    }

    void App::Draw()
    {
        for (auto& shape : mShapes) {
            switch (shape->type) {
            case BoxShape: {
                mDebug->drawBox(*(Box2d*)shape);
                break;
            }
            case EdgeShape: {
                mDebug->drawLine(*(Line2d*)shape);
                break;
            }
            case CircleShape: {
                mDebug->drawCircle(*(Circle2d*)shape);
                break;
            }
            }
        }

        // mDebug->drawBox(Box2d(capsule.x, capsule.y, capsule.w, capsule.h));
        Vec4f rect = { capsule.position, capsule.size };
        mBatch.submit(texture->mTexId, rect, texture->getUV(0));
        mBatch.begin();
        mBatch.draw();
        mBatch.end();

        mDebug->drawBox(capsule.getBox(), { 255,0,255,255 });
        mDebug->drawBox(rect);
        mDebug->drawCircle(capsule.getBCircle());
        mDebug->drawCircle(capsule.getTCircle());

        mDebug->end();
        mDebug->render();

        // mDebug->setColor({});
        // mDebug->drawGrid();
    }

    void App::Exit()
    {

    }
}