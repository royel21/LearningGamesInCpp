#include "PhysicSystem.h"
#include <ECS/Scene.h>
#include <ECS/Components.h>

namespace Plutus
{
    PhysicSystem::PhysicSystem(Scene* scene, vec2f gravity) : ISystem(scene) {
        mScene->getWorld()->SetGravity(b2Vec2{ gravity.x, gravity.y });
    }

    void PhysicSystem::init() {
        auto view = mScene->getRegistry()->view<Transform, RigidBody>();

        for (auto [ent, trans, rbody] : view.each()) {
            Entity Ent = { ent, mScene };
            auto& name = Ent.getName();
            auto world = mScene->getWorld();
            auto pos = toWorld(trans.getPosition() + rbody.mOffset);
            b2BodyDef body;
            body.type = (b2BodyType)rbody.mBodyType;
            body.position = pos;
            body.bullet = rbody.mBullet;
            body.fixedRotation = rbody.mFixedRotation;
            rbody.mBody = world->CreateBody(&body);
            body.linearDamping = 10;

            for (auto& fixture : rbody.mFixtures) {
                auto offset = toWorld(fixture.offset);

                b2FixtureDef fixDef;
                fixDef.density = fixture.density;
                fixDef.friction = fixture.friction;
                fixDef.restitution = fixture.restitution;

                switch (fixture.type) {
                case PBox: {
                    b2PolygonShape polygonShape;
                    b2Vec2 halfSize = { fixture.size.x * HMPP, fixture.size.y * HMPP };
                    polygonShape.SetAsBox(halfSize.x, halfSize.y, halfSize, 0); //a 2x2 rectangle

                    fixDef.shape = &polygonShape;
                    rbody.mBody->CreateFixture(&fixDef);
                    break;
                }
                case PLine: {
                    b2EdgeShape edgeShape;
                    auto bsize = toWorld(fixture.size);
                    edgeShape.SetTwoSided(pos, bsize);

                    fixDef.shape = &edgeShape;
                    rbody.mBody->CreateFixture(&fixDef);
                    break;
                }
                case PCircle: {
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
        mScene->updateWorld();

        auto view = mScene->getRegistry()->view<Transform, RigidBody>();
        for (auto [ent, trans, rbody] : view.each()) {
            if (rbody.mBodyType == DynamicBody || rbody.mBodyType == KinematicBody) {
                auto pos = fromWorld(rbody.mBody->GetPosition()) - rbody.mOffset;
                trans.x = pos.x;
                trans.y = pos.y;
            }
        }
    }



    void PhysicSystem::destroy() {
        mScene->resetWorld();
    }
} // namespace Plutus

