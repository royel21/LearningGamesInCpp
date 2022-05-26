#include "PhysicSystem.h"

#include <box2d/box2d.h>

#include <ECS/Scene.h>
#include <Log/Logger.h>
#include <ECS/Components.h>

#include <Math/Vectors.h>

#include <Core/Project.h>

#include <cmath>

namespace Plutus
{
    void createFixture(PhysicBodyComponent& pbody, const Vec2f& pos) {

        for (auto& fixture : pbody.mFixtures) {
            auto offset = toWorld(fixture.offset);

            b2FixtureDef fixDef;
            fixDef.density = fixture.density;
            fixDef.friction = fixture.friction;
            fixDef.restitution = fixture.restitution;
            fixDef.isSensor = fixture.isSensor;
            fixDef.filter.categoryBits = fixture.category;
            fixDef.filter.maskBits = fixture.mask;
            fixDef.filter.groupIndex = fixture.group;

            switch (fixture.type) {
            case P_Box: {
                b2PolygonShape polygonShape;

                b2Vec2 halfSize = { fixture.size.x * HMPP, fixture.size.y * HMPP };
                b2Vec2 center = { offset.x + halfSize.x, offset.y + halfSize.y };

                polygonShape.SetAsBox(halfSize.x, halfSize.y, center, 0); //a 2x2 rectangle

                fixDef.shape = &polygonShape;
                pbody.mBody->CreateFixture(&fixDef);
                break;
            }
            case P_Edge: {
                b2EdgeShape edgeShape;
                edgeShape.SetTwoSided(toWorld(pos + fixture.offset), toWorld(fixture.size + fixture.offset));

                fixDef.shape = &edgeShape;
                pbody.mBody->CreateFixture(&fixDef);
                break;
            }
            case P_Circle: {
                b2CircleShape circleShapeB;
                circleShapeB.m_p = offset;
                circleShapeB.m_radius = fixture.radius * PPM;;

                fixDef.shape = &circleShapeB;
                pbody.mBody->CreateFixture(&fixDef);
                break;
            }
            }

        } // end fixture forloop
    }

    void PhysicSystem::init(Project* project) {
        mProject = project;
        mWorld = new b2World({ mProject->gravity.x, mProject->gravity.y });
        mWorld->SetAutoClearForces(mProject->autoClearForce);
        mWorld->SetContactListener(this);

        auto view = mProject->scene->getRegistry()->view<TransformComponent, RigidBodyComponent>();
        for (auto [ent, trans, rbody] : view.each()) {
            auto pos = toWorld(trans.getPosition());

            b2BodyDef body;
            body.type = (b2BodyType)rbody.mBodyType;
            body.position = pos;
            body.bullet = rbody.mBullet;
            body.fixedRotation = rbody.mFixedRotation;
            body.linearDamping = rbody.mLinearDamping;
            body.gravityScale = rbody.mGravityScale;
            body.userData.pointer = (uintptr_t)ent;

            rbody.mBody = mWorld->CreateBody(&body);

            createFixture(rbody, trans.getPosition());
        } // end body forloop

        auto staticView = mProject->scene->getRegistry()->view<PhysicBodyComponent>();

        for (auto [ent, pbody] : staticView.each()) {
            Entity entity = { ent, mProject->scene.get() };
            auto trans = entity.getComponent<TransformComponent>();
            b2Vec2 pos = { 0, 0 };
            if (trans) {
                pos = toWorld(trans->getPosition());
            }

            b2BodyDef body;
            body.userData.pointer = (uintptr_t)ent;
            body.type = (b2BodyType)pbody.mBodyType;
            body.position = pos;
            pbody.mBody = mWorld->CreateBody(&body);
            createFixture(pbody, { pos.x, pos.y });
        }
    }

    void PhysicSystem::update(float dt) {
        mWorld->Step(mProject->timeStepInSec, mProject->velIter, mProject->positionIter);

        auto view = mProject->scene->getRegistry()->view<TransformComponent, RigidBodyComponent>();
        for (auto [ent, trans, rbody] : view.each()) {
            rbody.speedLimit();
            auto pos = fromWorld(rbody.mBody->GetPosition());
            trans.x = pos.x;
            trans.y = pos.y;
        }

        reportCollision();
    }

    void PhysicSystem::destroy() {
        if (mWorld != nullptr) delete mWorld;
        mWorld = nullptr;
    }

    void PhysicSystem::BeginContact(b2Contact* contact)
    {
        auto entA = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        bool sensorB = contact->GetFixtureA()->IsSensor();

        auto entB = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        bool sensorA = contact->GetFixtureB()->IsSensor();

        mCollisionsStart.push_back({ {entA, sensorB}, {entB, sensorA} });
    }

    void PhysicSystem::EndContact(b2Contact* contact)
    {
        auto entA = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        bool sensorB = contact->GetFixtureA()->IsSensor();

        auto entB = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        bool sensorA = contact->GetFixtureB()->IsSensor();

        mCollisionsEnd.push_back({ {entA, sensorB}, {entB, sensorA} });
    }

    void PhysicSystem::reportCollision() {
        for (auto& [first, second] : mCollisionsStart) {
            auto script1 = mProject->scene->getComponent<ScriptComponent>(first.entId);
            if (script1) script1->CollisionStart(second.entId, first.isSensor);

            auto script2 = mProject->scene->getComponent<ScriptComponent>(second.entId);
            if (script2) script2->CollisionStart(first.entId, second.isSensor);
        }

        for (auto& [first, second] : mCollisionsEnd) {
            auto script1 = mProject->scene->getComponent<ScriptComponent>(first.entId);
            if (script1) script1->CollisionEnd(second.entId, first.isSensor);

            auto script2 = mProject->scene->getComponent<ScriptComponent>(second.entId);
            if (script2) script2->CollisionEnd(first.entId, second.isSensor);
        }

        mCollisionsStart.clear();
        mCollisionsEnd.clear();
    }
} // namespace Plutus

