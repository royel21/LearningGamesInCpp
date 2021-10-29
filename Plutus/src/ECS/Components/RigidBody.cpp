#include "RigidBody.h"
#include "Transform.h"
namespace Plutus
{
    Body::~Body() {
        for (auto sh : shapes) {
            delete sh;
        }
        fixtures.clear();
    }

    void Body::init() {
        auto world = mEnt.mScene->getWorld();
        auto trans = mEnt.getComponent<Transform>();

        b2BodyDef body;
        body.type = (b2BodyType)bodyType;
        body.position = toWorld(trans->getPosition());
        body.bullet = bullet;
        body.fixedRotation = fixedRotation;
        mBody = world->CreateBody(&body);

        for (auto& fixture : fixtures) {
            auto offset = toWorld(fixture.offset);

            b2FixtureDef fixDef;
            fixDef.density = fixture.density;
            fixDef.friction = fixture.friction;
            fixDef.restitution = fixture.restitution;

            switch (fixture.type) {
            case PBox: {
                b2PolygonShape polygonShape;
                auto bsize = toWorld(fixture.size);
                polygonShape.SetAsBox(bsize.x, bsize.y, toWorld(fixture.size * 0.5f), 0); //a 2x2 rectangle

                fixDef.shape = &polygonShape;
                break;
            }
            case PLine: {
                b2PolygonShape polygonShape;
                auto bsize = toWorld(fixture.size);
                polygonShape.SetAsBox(bsize.x, bsize.y, toWorld(fixture.size * 0.5f), 0); //a 2x2 rectangle

                fixDef.shape = &polygonShape;
                break;
            }
            case PCircle: {
                b2CircleShape circleShapeB;
                circleShapeB.m_p = offset;
                circleShapeB.m_radius = fixture.radius * PPM;;

                fixDef.shape = &circleShapeB;
                break;
            }
            }

        }
    }

    void Body::addEdge(vec2f offset, vec2f size, float friction, float density, float restitution, bool isSensor) {
        fixtures.push_back({ PCircle, offset, size,0, friction, density, restitution, isSensor, });
    }

    void Body::addCircle(vec2f offset, float radius, float friction, float density, float restitution, bool isSensor) {
        fixtures.push_back({ PCircle, offset, vec2f{0,0}, radius, friction, density, restitution, isSensor, });
    }

    void Body::addBox(vec2f offset, vec2f size, float friction, float density, float restitution, bool isSensor) {
        fixtures.push_back({ PBox, offset, size, 0.0f, friction, density, restitution, isSensor, });
    }


} // namespace Plutus
