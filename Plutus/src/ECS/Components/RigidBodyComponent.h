#pragma once

#include "PhysicBodyComponent.h"

class b2Body;

#pragma once

namespace Plutus
{
    //Entity ent, BodyType type, const Vec2f& _offset = { 0,0 }, float liDamping = 0, float gScale = 1, float isFixedRotation = true, bool isBullet = false
    struct RigidBodyComponent : public PhysicBodyComponent {
        bool  mBullet = false;
        bool  mFixedRotation = true;
        float mLinearDamping = 0;
        float mGravityScale = 1;
        Vec2f mMaxVel;
        Vec2f mSpeedReducctionFactor = { 1.0f, 1.0f };

        RigidBodyComponent(BodyType type = DynamicBody) : PhysicBodyComponent(type) { };

        void ApplyForce(float x, float y);
        void ApplyImpulse(float x, float y);

        Vec2f getVelocity();
        void setVelocity(float x, float y);

        inline void setMaxVel(float x, float y = 0.0f) { mMaxVel = { x, y }; }

        void speedLimit();
    };
} // namespace Plutus