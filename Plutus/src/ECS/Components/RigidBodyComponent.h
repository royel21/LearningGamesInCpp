#pragma once
#include <vector>

#include <box2d/box2d.h>
#include <Physics/Shapes.h>
#include <Math/Vectors.h>
#include "../Scene.h"

class b2Body;

#pragma once

namespace Plutus
{
    enum BodyType {
        StaticBody = b2_staticBody,
        DynamicBody = b2_dynamicBody,
        KinematicBody = b2_kinematicBody
    };

    struct Fixture {
        int type;

        vec2f offset;
        vec2f size = { 1,1 };
        float radius = 1;
        float friction = 0.3f;
        float density = 1.0f;
        float restitution = 0;
        bool isSensor = false;
        Fixture(int _type) : type(_type) {};
        Fixture(int _type, vec2f _offset, vec2f _size, float r, float f, float d, float res, bool _isSensor)
            : type(_type), offset(_offset), size(_size), friction(f), radius(r), density(d), restitution(res), isSensor(_isSensor)
        {

        }
    };

    class RigidBodyComponent {
    public:
        bool mBullet = false;
        bool mFixedRotation = true;
        float linearDamping = 0;
        float gravityScale = 1;

        Entity mEnt;
        vec2f mOffset;
        b2Body* mBody;
        BodyType mBodyType = DynamicBody;
        std::vector<Fixture> mFixtures;

    public:
        RigidBodyComponent() = default;
        RigidBodyComponent(Entity ent, BodyType type, const vec2f& _offset = { 0,0 }, float liDamping = 0, float gScale = 1, float isFixedRotation = true, bool isBullet = false);

        void init();
        void addEdge(vec2f offset, vec2f size, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false);
        void addCircle(vec2f offset, float radius, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false);
        void addBox(vec2f offset, vec2f size, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false);

        void ApplyForce(float x, float y);
        void ApplyImpulse(float x, float y);
        void setVelocity(float x, float y);

    };
} // namespace Plutus