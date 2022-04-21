#pragma once

#include "PhysicBodyComponent.h"

class b2Body;

#pragma once

namespace Plutus
{
    //Entity ent, BodyType type, const vec2f& _offset = { 0,0 }, float liDamping = 0, float gScale = 1, float isFixedRotation = true, bool isBullet = false
    struct RigidBodyComponent : public PhysicBodyComponent {
        bool  mBullet = false;
        bool  mFixedRotation = true;
        float mLinearDamping = 0;
        float mGravityScale = 1;
        vec2f mMaxVel;
        RigidBodyComponent(BodyType type = DynamicBody) : PhysicBodyComponent(mBodyType) { };

        void ApplyForce(float x, float y);
        void ApplyImpulse(float x, float y);

        vec2f getVelocity();
        void setVelocity(float x, float y);

        inline void setMaxVel(float x, float y = 0.0f) { mMaxVel = { x, y }; }

        void speedLimit();
    };
} // namespace Plutus