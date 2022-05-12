#include "PhysicSystem.h"

#include <box2d/box2d.h>

#include <ECS/Scene.h>
#include <Log/Logger.h>
#include <ECS/Components.h>

#include <Math/Vectors.h>

#include <Core/Project.h>

#include <cmath>

constexpr float PIXEL_SIZE = 100.0f;
//Pixel Per Meter
constexpr float PPM = 1 / PIXEL_SIZE;
//Meter Per Pixel
constexpr float MPP = PIXEL_SIZE;
//Half Meter Per Pixel
constexpr float HMPP = PPM / 2.0f;


namespace Plutus
{
    inline b2Vec2 toWorld(const Vec2f& value) {
        return { value.x * PPM, value.y * PPM };
    }

    inline Vec2f fromWorld(b2Vec2 value) {
        return { value.x * MPP, value.y * MPP };
    }

    inline b2Vec2 tobVec2(const Vec2f& vec) { return { vec.x, vec.y }; }
    inline Vec2f toVec2f(const b2Vec2& vec) { return { vec.x, vec.y }; }

    void createFixture(PhysicBodyComponent& pbody, const Vec2f& pos) {

        for (auto& fixture : pbody.mFixtures) {
            auto offset = toWorld(fixture.offset);

            b2FixtureDef fixDef;
            fixDef.density = fixture.density;
            fixDef.friction = fixture.friction;
            fixDef.restitution = fixture.restitution;
            fixDef.isSensor = fixture.isSensor;

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

            rbody.mBody = mWorld->CreateBody(&body);

            createFixture(rbody, trans.getPosition());
        } // end body forloop

        auto staticView = mProject->scene->getRegistry()->view<PhysicBodyComponent>();

        for (auto [ent, pbody] : staticView.each()) {
            Entity ent = { ent, mProject->scene.get() };
            auto trans = ent.getComponent<TransformComponent>();
            b2Vec2 pos = { 0, 0 };
            if (trans) {
                pos = toWorld(trans->getPosition());
            }

            b2BodyDef body;
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
            auto pos = fromWorld(rbody.mBody->GetPosition());
            trans.x = pos.x;
            trans.y = pos.y;
            rbody.speedLimit();
        }
    }

    void PhysicSystem::destroy() {
        if (mWorld != nullptr) delete mWorld;
        mWorld = nullptr;
    }
} // namespace Plutus

