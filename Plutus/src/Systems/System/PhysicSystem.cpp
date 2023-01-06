#include "PhysicSystem.h"
#include "EventListener/ICollisionListener.h"

#include <cmath>

#include <ECS/Scene.h>
#include <Log/Logger.h>
#include <Core/Project.h>


namespace Plutus
{

    void createFixture(PhysicBodyComponent& pbody, const Vec2f& pos);

    void PhysicSystem::init() {
        mWorld = new b2World({ mProject->gravity.x, mProject->gravity.y });
        mWorld->SetAutoClearForces(mProject->autoClearForce);
        mWorld->SetContactListener(this);

        mCollisionsStart.reserve(20);
        mCollisionsEnd.reserve(20);

        auto view = mProject->scene->getRegistry()->view<TransformComponent, RigidBodyComponent>();

        b2BodyDef body;
        for (auto [ent, trans, rbody] : view.each()) {
            auto pos = trans.getPosition();

            body.type = (b2BodyType)rbody.mBodyType;
            body.position = toWorld(pos);
            body.bullet = rbody.mBullet;
            body.fixedRotation = rbody.mFixedRotation;
            body.linearDamping = rbody.mLinearDamping;
            body.gravityScale = rbody.mGravityScale;
            body.userData.pointer = (uintptr_t)ent;

            rbody.mBody = mWorld->CreateBody(&body);

            createFixture(rbody, pos);
        } // end body forloop

        auto staticView = mProject->scene->getRegistry()->view<PhysicBodyComponent>();

        for (auto [ent, pbody] : staticView.each()) {
            auto trans = mProject->scene->getComponent<TransformComponent>(ent);
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

    void createFixture(PhysicBodyComponent& pbody, const Vec2f& pos) {

        b2FixtureDef fixDef;
        b2PolygonShape polygonShape;
        b2EdgeShape edgeShape;
        b2CircleShape circleShapeB;

        for (auto& fixture : pbody.mFixtures) {
            auto offset = toWorld(fixture.offset);

            fixDef.density = fixture.density;
            fixDef.friction = fixture.friction;
            fixDef.restitution = fixture.restitution;
            fixDef.isSensor = fixture.isSensor;
            fixDef.filter.categoryBits = fixture.category;
            fixDef.filter.maskBits = fixture.mask;
            fixDef.filter.groupIndex = fixture.group;

            switch (fixture.type) {
            case P_Box: {

                b2Vec2 halfSize = { fixture.size.x * HMPP, fixture.size.y * HMPP };
                b2Vec2 center = { offset.x + halfSize.x, offset.y + halfSize.y };

                polygonShape.SetAsBox(halfSize.x, halfSize.y, center, 0); //a 2x2 rectangle

                fixDef.shape = &polygonShape;
                break;
            }
            case P_Edge: {
                edgeShape.SetTwoSided(toWorld(pos + fixture.offset), toWorld(fixture.size + fixture.offset));

                fixDef.shape = &edgeShape;
                break;
            }
            case P_Circle: {
                circleShapeB.m_p = offset;
                circleShapeB.m_radius = fixture.radius * PPM;;

                fixDef.shape = &circleShapeB;
                break;
            }
            }
            fixture.b2fixture = pbody.mBody->CreateFixture(&fixDef);

        } // end fixture forloop
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

        for (auto& [first, second] : mCollisionsStart) {
            for (auto listener : mCollisionListener) {
                listener->CollisionEvent(first.entId, first.isSensor, second.entId, second.isSensor);
            }
        }

        for (auto& [first, second] : mCollisionsEnd) {
            for (auto listener : mCollisionListener) {
                listener->CollisionEvent(first.entId, first.isSensor, second.entId, second.isSensor, false);
            }
        }

        mCollisionsStart.clear();
        mCollisionsEnd.clear();
    }


    bool PhysicSystem::ReportFixture(b2Fixture* fix) {
        if (fix->GetFilterData().categoryBits & mQueryMask)
            mEntities.push_back(fix->GetBody()->GetUserData().pointer);
        return true;
    }


    void PhysicSystem::removeListener(ICollisionListener* listener) {
        auto it = std::find_if(mCollisionListener.begin(), mCollisionListener.end(), [&](ICollisionListener* lstn) {
            return listener == lstn;
            });

        if (it != mCollisionListener.end()) mCollisionListener.erase(it);
    }

    void PhysicSystem::destroy() {
        if (mWorld != nullptr) delete mWorld;
        mWorld = nullptr;
        mCollisionListener.clear();
    }

    void PhysicSystem::BeginContact(b2Contact* contact)
    {
        auto entA = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        bool sensorA = contact->GetFixtureA()->IsSensor();

        auto entB = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        bool sensorB = contact->GetFixtureB()->IsSensor();

        mCollisionsStart.push_back({ {entA, sensorA}, {entB, sensorB} });
    }

    void PhysicSystem::EndContact(b2Contact* contact)
    {
        auto entA = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        bool sensorA = contact->GetFixtureA()->IsSensor();

        auto entB = contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        bool sensorB = contact->GetFixtureB()->IsSensor();

        mCollisionsEnd.push_back({ {entA, sensorA}, {entB, sensorB} });
    }
} // namespace Plutus

