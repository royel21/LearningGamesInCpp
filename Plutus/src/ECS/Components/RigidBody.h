#pragma once
#include <vector>

#include <Box2d/box2d.h>
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
        ShapeType type;

        vec2f offset;
        vec2f size;
        float radius;
        float friction = 0;
        float density = 1.0f;
        float restitution = 0;
        bool isSensor = false;
        Fixture(ShapeType _type, vec2f _offset, vec2f _size, float r, float f, float d, float res, bool _isSensor)
            : type(_type), offset(_offset), size(_size), friction(f), radius(r), density(d), restitution(res), isSensor(_isSensor)
        {

        }
    };

    class RigidBody {
    public:
        Entity mEnt;
        b2Body* mBody;
        BodyType mBodyType;
        std::vector<Fixture> mFixtures;
        vec2f mOffset;
        bool mFixedRotation = true;
        float mBullet = false;

    public:
        RigidBody(Entity ent, BodyType type, const vec2f& _offset = { 0,0 }, float isFixedRotation = true, bool isBullet = false);

        void init();
        void addEdge(vec2f offset, vec2f size, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false);
        void addCircle(vec2f offset, float radius, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false);
        void addBox(vec2f offset, vec2f size, float friction = 0.3f, float density = 1.0f, float restitution = 0, bool isSensor = false);

        void ApplyForce(float x, float y);
        void ApplyImpulse(float x, float y);

    };
} // namespace Plutus