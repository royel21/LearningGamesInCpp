#include "RigidBody.h"
#include "Transform.h"
namespace Plutus
{
    RigidBody::RigidBody(Entity ent, BodyType type, const vec2f& offset, float liDamping, float gScale, float isFixedRotation, bool isBullet) :
        mEnt(ent), mBodyType(type), mOffset(offset), linearDamping(liDamping), gravityScale(gScale), mFixedRotation(isFixedRotation), mBullet(isBullet) {
    }

    void RigidBody::addEdge(vec2f offset, vec2f size, float friction, float density, float restitution, bool isSensor) {
        mFixtures.push_back({ PCircle, offset, size,0, friction, density, restitution, isSensor, });
    }

    void RigidBody::addCircle(vec2f offset, float radius, float friction, float density, float restitution, bool isSensor) {
        mFixtures.push_back({ PCircle, offset, vec2f{0,0}, radius, friction, density, restitution, isSensor, });
    }

    void RigidBody::addBox(vec2f offset, vec2f size, float friction, float density, float restitution, bool isSensor) {
        mFixtures.push_back({ PBox, offset, size, 0.0f, friction, density, restitution, isSensor, });
    }

    void RigidBody::ApplyForce(float x, float y) {
        mBody->ApplyForceToCenter(toWorld({ x, y }), true);
    }

    void RigidBody::ApplyImpulse(float x, float y) {
        mBody->ApplyLinearImpulseToCenter(toWorld({ x, y }), true);
    }

    void RigidBody::setVelocity(float x, float y) {
        mBody->SetLinearVelocity(b2Vec2(x, y));
    }
} // namespace Plutus
