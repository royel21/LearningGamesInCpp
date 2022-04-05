#include "RigidBodyComponent.h"

namespace Plutus
{
    RigidBodyComponent::RigidBodyComponent(Entity ent, BodyType type, const vec2f& offset, float liDamping, float gScale, float isFixedRotation, bool isBullet) :
        mEnt(ent), mBodyType(type), mOffset(offset), linearDamping(liDamping), gravityScale(gScale), mFixedRotation(isFixedRotation), mBullet(isBullet) {
    }

    void RigidBodyComponent::addEdge(vec2f offset, vec2f size, float friction, float density, float restitution, bool isSensor) {
        mFixtures.push_back({ EdgeShape, offset, size,0, friction, density, restitution, isSensor, });
    }

    void RigidBodyComponent::addCircle(vec2f offset, float radius, float friction, float density, float restitution, bool isSensor) {
        mFixtures.push_back({ CircleShape, offset, vec2f{0,0}, radius, friction, density, restitution, isSensor, });
    }

    void RigidBodyComponent::addBox(vec2f offset, vec2f size, float friction, float density, float restitution, bool isSensor) {
        mFixtures.push_back({ BoxShape, offset, size, 0.0f, friction, density, restitution, isSensor, });
    }

    void RigidBodyComponent::ApplyForce(float x, float y) {
        mBody->ApplyForceToCenter(toWorld({ x, y }), true);
    }

    void RigidBodyComponent::ApplyImpulse(float x, float y) {
        mBody->ApplyLinearImpulseToCenter(toWorld({ x, y }), true);
    }

    void RigidBodyComponent::setVelocity(float x, float y) {
        mBody->SetLinearVelocity(b2Vec2(x, y));
    }
} // namespace Plutus
