#include "PhysicSystem.h"
#include <ECS/Scene.h>
#include <ECS/Components.h>
#include <Log/Logger.h>

namespace Plutus
{

    void PhysicSystem::init() {
        auto gravity = mScene->getGravity();
        mWorld = new b2World({ gravity.x, gravity.y });
        mWorld->SetAutoClearForces(mScene->getAutoClearForce());

        auto view = mScene->getRegistry()->view<TransformComponent, RigidBodyComponent>();
        for (auto [ent, trans, rbody] : view.each()) {
            Entity Ent = { ent, mScene };

            auto pos = toWorld(trans.getPosition());

            b2BodyDef body;
            body.type = (b2BodyType)rbody.mBodyType;
            body.position = pos;
            body.bullet = rbody.mBullet;
            body.fixedRotation = rbody.mFixedRotation;
            body.linearDamping = rbody.mLinearDamping;
            body.gravityScale = rbody.mGravityScale;

            rbody.mBody = mWorld->CreateBody(&body);

            // rbody.mBody.
            for (auto& fixture : rbody.mFixtures) {
                auto offset = toWorld(fixture.offset);

                b2FixtureDef fixDef;
                fixDef.density = fixture.density;
                fixDef.friction = fixture.friction;
                fixDef.restitution = fixture.restitution;
                fixDef.isSensor = fixture.isSensor;

                switch (fixture.type) {
                case BoxShape: {
                    b2PolygonShape polygonShape;

                    b2Vec2 halfSize = { fixture.size.x * HMPP, fixture.size.y * HMPP };
                    b2Vec2 center = { offset.x + halfSize.x, offset.y + halfSize.y };

                    polygonShape.SetAsBox(halfSize.x, halfSize.y, center, 0); //a 2x2 rectangle

                    fixDef.shape = &polygonShape;
                    rbody.mBody->CreateFixture(&fixDef);
                    break;
                }
                case EdgeShape: {
                    b2EdgeShape edgeShape;
                    auto bsize = toWorld({ fixture.size.x + offset.x, fixture.size.y + offset.y });
                    edgeShape.SetTwoSided({ offset.x + pos.x, offset.y + pos.y }, bsize);

                    fixDef.shape = &edgeShape;
                    rbody.mBody->CreateFixture(&fixDef);
                    break;
                }
                case CircleShape: {
                    b2CircleShape circleShapeB;
                    circleShapeB.m_p = offset;
                    circleShapeB.m_radius = fixture.radius * PPM;;

                    fixDef.shape = &circleShapeB;
                    rbody.mBody->CreateFixture(&fixDef);
                    break;
                }
                }
            } // end fixture forloop
        } // end body forloop
    }

    void PhysicSystem::update(float dt) {
        mWorld->Step(mScene->getTimeIterSec(), mScene->getVelIter(), mScene->getPositionIter());

        auto view = mScene->getRegistry()->view<TransformComponent, RigidBodyComponent>();
        for (auto [ent, trans, rbody] : view.each()) {
            if (rbody.mBodyType == DynamicBody || rbody.mBodyType == KinematicBody) {

                auto pos = fromWorld(rbody.mBody->GetPosition());
                trans.x = pos.x;
                trans.y = pos.y;
                rbody.speedLimit();
            }
        }
    }

    void PhysicSystem::destroy() {
        if (mWorld != nullptr) delete mWorld;
        mWorld = nullptr;
    }
} // namespace Plutus

